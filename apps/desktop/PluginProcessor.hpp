#pragma once
#include "algorhythmia/host/processor.hpp"
#include "algorhythmia/host/state.hpp"

#include <bit>
#include <juce_audio_utils/juce_audio_utils.h>
#include <mutex>

namespace sys01 {
static_assert(std::atomic<float>::is_always_lock_free);
namespace contract = algorhythmia::host;
// All parameter writers enter virtual setValue, including APVTS attachments,
// undo/tree replay, restore, native host setters and notifying listener reentry.
// The value and unique writer ticket share one atomic publication word. AU uses
// one CAS, with no retry/wait. External exchange defines writer linearization;
// ticket allocation order need not equal exchange order. After 2^32-1 tickets
// conditional publication permanently fails closed, preventing generation ABA.
class ControlPublication {
public:
    explicit ControlPublication(float value) : word_(std::bit_cast<std::uint32_t>(value)) {}
    float value() const noexcept {
        return std::bit_cast<float>(
            static_cast<std::uint32_t>(word_.load(std::memory_order_acquire)));
    }
    std::uint64_t capture() const noexcept {
        const auto word = word_.load(std::memory_order_acquire);
        return exhausted_.load(std::memory_order_acquire) ? word | 0xffffffff00000000ULL : word;
    }
    void write(float value) noexcept {
        const auto ticket = tickets_.fetch_add(1, std::memory_order_relaxed) + 1;
        if (ticket >= UINT32_MAX)
            exhausted_.store(true, std::memory_order_release);
        const auto generation = std::min<std::uint64_t>(ticket, UINT32_MAX);
        word_.exchange((generation << 32) | std::bit_cast<std::uint32_t>(value),
                       std::memory_order_acq_rel);
    }
    bool publish(float value, std::uint64_t captured) noexcept {
        if (exhausted_.load(std::memory_order_acquire) || (captured >> 32) == UINT32_MAX)
            return false;
        auto current = word_.load(std::memory_order_acquire);
        if ((current >> 32) != (captured >> 32))
            return false;
        const auto desired =
            (current & 0xffffffff00000000ULL) | std::bit_cast<std::uint32_t>(value);
        return word_.compare_exchange_strong(current, desired, std::memory_order_acq_rel,
                                             std::memory_order_acquire);
    }

private:
    std::atomic<std::uint64_t> word_, tickets_{0};
    std::atomic<bool> exhausted_{false};
};
static_assert(std::atomic<std::uint64_t>::is_always_lock_free);
class PublishedParameter {
public:
    virtual ~PublishedParameter() = default;
    virtual std::uint64_t capturePublication() const noexcept = 0;
    virtual bool publishCaptured(float, std::uint64_t) noexcept = 0;
};
// Keep JUCE's IDs, ranges, defaults, text and discrete metadata. Own only scalar
// storage; nobody in SYS 01 uses the concrete base's get()/assignment shortcuts.
template <typename Base> class Parameter final : public Base, public PublishedParameter {
public:
    template <typename... Args>
    explicit Parameter(float initial, Args&&... args)
        : Base(std::forward<Args>(args)...), publication_(initial) {}
    float getValue() const override {
        return publication_.value();
    }
    void setValue(float value) override {
        publication_.write(this->convertTo0to1(this->convertFrom0to1(value)));
    }
    std::uint64_t capturePublication() const noexcept override {
        return publication_.capture();
    }
    bool publishCaptured(float value, std::uint64_t captured) noexcept override {
        return publication_.publish(value, captured);
    }

private:
    ControlPublication publication_;
};

// One UI/session producer (serialized off audio), one audio consumer. Slots
// become reusable only after consumer release. No retry loops on audio.
template <typename T, std::uint32_t Capacity> class Queue {
public:
    bool push(const T& value) noexcept {
        const auto w = write_.load(std::memory_order_relaxed);
        const auto next = (w + 1) % Capacity;
        if (next == read_.load(std::memory_order_acquire))
            return false;
        slots_[w] = value;
        write_.store(next, std::memory_order_release);
        return true;
    }
    bool peek(T& value, std::uint32_t offset) const noexcept {
        const auto r = read_.load(std::memory_order_relaxed);
        const auto w = write_.load(std::memory_order_acquire);
        const auto available = (w + Capacity - r) % Capacity;
        if (offset >= available)
            return false;
        value = slots_[(r + offset) % Capacity];
        return true;
    }
    void consume(std::uint32_t count) noexcept {
        read_.store((read_.load(std::memory_order_relaxed) + count) % Capacity,
                    std::memory_order_release);
    }
    std::uint32_t boundary() const noexcept {
        return write_.load(std::memory_order_acquire);
    }
    bool pop(T& value) noexcept {
        return pop(value, boundary());
    }
    bool pop(T& value, std::uint32_t boundary) noexcept {
        const auto r = read_.load(std::memory_order_relaxed);
        if (r == boundary)
            return false;
        value = slots_[r];
        read_.store((r + 1) % Capacity, std::memory_order_release);
        return true;
    }

private:
    std::array<T, Capacity> slots_{};
    std::atomic<std::uint32_t> read_{0}, write_{0};
};
// Immutable block result. One audio producer, one message-thread consumer.
// Full mailbox drops new snapshots; never overwrites a reader's slot or waits.
struct TelemetrySnapshot {
    std::uint64_t block = 0;
    std::uint32_t frames = 0, rejections = 0, droppedSnapshots = 0;
    float sampleRate = 48000;
    contract::Status audio{};
};
static_assert(std::is_trivially_copyable_v<TelemetrySnapshot>);
class PluginProcessor final : public juce::AudioProcessor {
public:
    PluginProcessor();
    void prepareToPlay(double, int) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void processBlockBypassed(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override {
        return true;
    }
    const juce::String getName() const override {
        return "SYS 01";
    }
    bool acceptsMidi() const override {
        return true;
    }
    bool producesMidi() const override {
        return false;
    }
    bool isMidiEffect() const override {
        return false;
    }
    double getTailLengthSeconds() const override {
        return std::numeric_limits<double>::infinity();
    }
    int getNumPrograms() override {
        return static_cast<int>(contract::presets.size());
    }
    int getCurrentProgram() override {
        return program_.load();
    }
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    juce::AudioProcessorParameter* getBypassParameter() const override;
    contract::State controls() const noexcept;
    bool restore(const contract::State&); // non-audio, coherent reset transaction
    // VST3 audio-owner ingress; called only by the pinned format integration.
    void beginNativeBlock(int frames) noexcept;
    bool nativePoint(int parameterIndex, int offset, float value) noexcept;
    // AU captures scalar controls with its ingress epoch. Later producers must
    // not contaminate that block's host-target acknowledgement. Audio owner only.
    std::uint64_t parameterPublication(std::size_t i) const noexcept;
    bool publishNativeParameter(std::size_t i, float value, std::uint64_t captured);
    std::uint32_t controlEpoch() const noexcept {
        return epoch_.load(std::memory_order_acquire);
    }
    void nativeControlsSnapshot(const contract::State& state, std::uint32_t epoch) noexcept {
        nativeControls_ = state;
        nativeEpoch_ = epoch;
        hasNativeControls_ = true;
    }
    void rejectNativeBlock() noexcept {
        nativeRejected_ = true;
    }
    bool nativeMidiCC(int offset, int cc, int value) noexcept;
    TelemetrySnapshot readTelemetry() noexcept; // single message-thread consumer
    bool button(bool drive, bool pressed);      // UI thread only; bounded queue
    bool clipped() const noexcept {
        return clipped_.load(std::memory_order_relaxed) != 0;
    }
    float inputPeak() const noexcept;
    float outputPeak() const noexcept;
    algorhythmia::Telemetry telemetry() const noexcept;
    std::uint32_t rejected() const noexcept {
        return rejected_.load();
    }
    juce::UndoManager undo;
    juce::AudioProcessorValueTreeState parameters;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout layout();
    contract::Processor processor_;
    contract::State cached_;
    std::array<contract::Event, contract::eventCapacity> nativeEvents_{};
    std::array<contract::Event, contract::eventCapacity> nativeMidi_{};
    std::size_t nativeMidiSize_ = 0;
    std::array<bool, contract::count> nativeMask_{};
    std::size_t nativeSize_ = 0;
    int nativeFrames_ = 0;
    bool nativeRejected_ = false;
    contract::State nativeControls_;
    bool hasNativeControls_ = false;
    std::uint32_t nativeEpoch_ = 0;
    Queue<TelemetrySnapshot, 9> telemetryQueue_;
    TelemetrySnapshot uiTelemetry_{};
    float sampleRate_ = 48000;
    std::uint64_t blockSequence_ = 0;
    std::uint32_t droppedSnapshots_ = 0;
    Queue<contract::State, 9> restores_;
    Queue<contract::Event, 65> buttons_;
    std::mutex stateMutex_; // never touched by processBlock
    std::atomic<std::uint32_t> epoch_{0}, input_{0}, output_{0}, display_{0}, rejected_{0},
        clipped_{0};
    std::atomic<int> program_{0};
    void process(juce::AudioBuffer<float>&, juce::MidiBuffer&, bool);
};
} // namespace sys01
