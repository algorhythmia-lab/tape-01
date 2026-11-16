#include "PluginProcessor.hpp"

#include "PluginEditor.hpp"

#include <bit>

namespace sys01 {
juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::layout() {
    juce::AudioProcessorValueTreeState::ParameterLayout result;
    for (const auto& p : contract::catalogue) {
        const juce::ParameterID key(juce::String(p.key.data()), 1);
        const juce::String label(p.label.data());
        if (p.id == contract::Id::Domain) {
            result.add(std::make_unique<Parameter<juce::AudioParameterChoice>>(
                p.initial, key, label,
                juce::StringArray{"00 / BYTEBEAT", "01 / CELLULAR FM", "02 / LIVE TRANSFORM"}, 0));
        } else if (p.steps == 2) {
            result.add(std::make_unique<Parameter<juce::AudioParameterBool>>(p.initial, key, label,
                                                                             p.initial != 0));
        } else {
            result.add(std::make_unique<Parameter<juce::AudioParameterFloat>>(
                p.initial, key, label, juce::NormalisableRange<float>(0, 1), p.initial));
        }
    }
    return result;
}
PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("INPUT", juce::AudioChannelSet::stereo(), true)
                         .withOutput("OUTPUT", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, &undo, "SYS01", layout()) {}
contract::State PluginProcessor::controls() const noexcept {
    contract::State state;
    for (const auto& p : contract::catalogue) {
        // Authoritative publication word, not a listener's delayed raw mirror.
        const auto value = parameters.getParameter(p.key.data())->getValue();
        state.values[contract::index(p.id)] = value;
    }
    return state;
}
std::uint64_t PluginProcessor::parameterPublication(std::size_t i) const noexcept {
    return dynamic_cast<PublishedParameter*>(getParameters()[static_cast<int>(i)])
        ->capturePublication();
}
bool PluginProcessor::publishNativeParameter(std::size_t i, float value, std::uint64_t captured) {
    auto* parameter = getParameters()[static_cast<int>(i)];
    if (!dynamic_cast<PublishedParameter*>(parameter)->publishCaptured(value, captured))
        return false;
    // setValueNotifyingHost would reenter an unconditional writer. Notify only
    // after CAS. APVTS's pinned listener reads current getValue(), so reentrant
    // writes retain authority even when an older notification finishes later.
    parameter->sendValueChangedMessageToListeners(parameter->getValue());
    return true;
}
void PluginProcessor::prepareToPlay(double rate, int) {
    // Host lifecycle excludes the callback. Parameters survive graph rebuild;
    // audio history is deliberately reset on prepare/sample-rate change.
    sampleRate_ = algorhythmia::sanitizeRate(static_cast<float>(rate));
    cached_ = controls();
    processor_.prepare(static_cast<float>(rate));
    static_cast<void>(processor_.restore(cached_));
}
bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& l) const {
    return l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           (l.getMainInputChannelSet() == juce::AudioChannelSet::stereo() ||
            l.getMainInputChannelSet().isDisabled());
}
juce::AudioProcessorParameter* PluginProcessor::getBypassParameter() const {
    return parameters.getParameter("bypass");
}
bool PluginProcessor::restore(const contract::State& state) {
    if (!contract::valid(state))
        return false;
    std::lock_guard lock(stateMutex_);
    // Reserve transaction before publishing UI controls. A full queue leaves
    // controls intact. Audio will not observe a partial setter sequence.
    epoch_.fetch_add(1, std::memory_order_acq_rel);
    if (!restores_.push(state)) {
        epoch_.fetch_add(1, std::memory_order_release);
        rejected_.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    for (const auto& p : contract::catalogue) {
        auto* parameter = parameters.getParameter(p.key.data());
        parameter->beginChangeGesture();
        parameter->setValueNotifyingHost(state.values[contract::index(p.id)]);
        parameter->endChangeGesture();
    }
    epoch_.fetch_add(1, std::memory_order_release);
    return true;
}
bool PluginProcessor::button(bool drive, bool pressed) {
    const auto ok =
        buttons_.push({0, drive ? contract::EventKind::Brake : contract::EventKind::StateEdge,
                       contract::Id::Flux, pressed ? 1.0F : 0.0F});
    if (!ok)
        rejected_.fetch_add(1, std::memory_order_relaxed);
    return ok;
}
void PluginProcessor::beginNativeBlock(int frames) noexcept {
    nativeSize_ = 0;
    nativeMidiSize_ = 0;
    nativeFrames_ = frames;
    nativeMask_ = {};
    nativeRejected_ = false;
    hasNativeControls_ = false;
}
bool PluginProcessor::nativePoint(int parameterIndex, int offset, float value) noexcept {
    // Synthetic JUCE program parameters remain lifecycle operations. They are
    // not members of the musical catalogue and are not position-automatable.
    if (parameterIndex < 0 || static_cast<std::size_t>(parameterIndex) >= contract::count)
        return false;
    if (nativeFrames_ == 0)
        return false; // zero-block flush updates the host target only
    if (nativeSize_ + nativeMidiSize_ == nativeEvents_.size() || offset < 0 ||
        offset >= nativeFrames_ || !std::isfinite(value) || value < 0 || value > 1) {
        nativeRejected_ = true;
        return false;
    }
    const auto i = static_cast<std::size_t>(parameterIndex);
    const auto& p = contract::catalogue[i];
    if (p.steps > 0)
        value =
            std::round(value * static_cast<float>(p.steps - 1)) / static_cast<float>(p.steps - 1);
    nativeEvents_[nativeSize_++] = {static_cast<std::size_t>(offset),
                                    contract::EventKind::Parameter, p.id, value};
    nativeMask_[i] = true;
    return true;
}
bool PluginProcessor::nativeMidiCC(int offset, int cc, int value) noexcept {
    if (nativeFrames_ == 0)
        return false;
    if (offset < 0 || offset >= nativeFrames_ || cc < 0 || cc > 127 || value < 0 || value > 127) {
        nativeRejected_ = true;
        return false;
    }
    contract::Id id;
    float normalized;
    if (cc == 20 || cc == 21) {
        id = cc == 20 ? contract::Id::Flux : contract::Id::Bias;
        normalized = static_cast<float>(value) / 127;
    } else if (cc == 22) {
        id = contract::Id::Drive;
        normalized = value >= 64 ? 1.0F : 0.0F;
    } else if (cc == 23) {
        id = contract::Id::Domain;
        normalized = static_cast<float>(static_cast<unsigned>(value) * 3U / 128U) / 2;
    } else
        return true; // outside the declared musical input
    if (nativeSize_ + nativeMidiSize_ == nativeEvents_.size()) {
        nativeRejected_ = true;
        return false;
    }
    nativeMidi_[nativeMidiSize_++] = {static_cast<std::size_t>(offset),
                                      contract::EventKind::Parameter, id, normalized};
    return true;
}
TelemetrySnapshot PluginProcessor::readTelemetry() noexcept {
    const auto boundary = telemetryQueue_.boundary();
    for (unsigned i = 0; i < 8 && telemetryQueue_.pop(uiTelemetry_, boundary); ++i) {
    }
    return uiTelemetry_;
}
void PluginProcessor::processBlock(juce::AudioBuffer<float>& b, juce::MidiBuffer& midi) {
    process(b, midi, false);
}
void PluginProcessor::processBlockBypassed(juce::AudioBuffer<float>& b, juce::MidiBuffer& midi) {
    process(b, midi, true);
}
void PluginProcessor::process(juce::AudioBuffer<float>& b, juce::MidiBuffer& midi,
                              bool hostBypass) {
    juce::ScopedNoDenormals noDenormals;
    if (b.getNumSamples() == 0)
        return;
    if (b.getNumChannels() < 2) {
        b.clear();
        return;
    }
    const auto frames = static_cast<std::size_t>(b.getNumSamples());
    std::array<contract::Event, contract::eventCapacity> events{};
    std::size_t size = 0;
    bool overflow = nativeRejected_;
    const auto add = [&](const contract::Event& e) {
        if (size == events.size()) {
            overflow = true;
            return;
        }
        events[size++] = e;
    };
    const auto before = epoch_.load(std::memory_order_acquire);
    const auto restoreBoundary = restores_.boundary();
    auto candidate = cached_;
    bool snapshotValid = false;
    if ((before & 1U) == 0)
        candidate = hasNativeControls_ ? nativeControls_ : controls();
    const auto after = epoch_.load(std::memory_order_acquire);
    if (before == after && (after & 1U) == 0 && (!hasNativeControls_ || before == nativeEpoch_)) {
        contract::State restored;
        bool hasRestore = false;
        for (unsigned i = 0; i < 8 && restores_.pop(restored, restoreBoundary); ++i)
            hasRestore = true;
        if (hasRestore) {
            static_cast<void>(processor_.restore(restored));
            cached_ = restored;
        }
        snapshotValid = true;
        for (const auto& p : contract::catalogue) {
            const auto i = contract::index(p.id);
            if (p.id != contract::Id::Bypass && !nativeMask_[i] &&
                candidate.values[i] != cached_.values[i])
                add({0, contract::EventKind::Parameter, p.id, candidate.values[i]});
        }
    }
    // Only a changed bypass target consumes a slot; all 256 native points fit.
    // Positioned bypass points take precedence over the final host target.
    const auto bypassIndex = contract::index(contract::Id::Bypass);
    const auto bypassTarget = hostBypass ? 1.0F : candidate.values[bypassIndex];
    if (!nativeMask_[bypassIndex] && cached_.values[bypassIndex] != bypassTarget)
        add({0, contract::EventKind::Parameter, contract::Id::Bypass, bypassTarget});
    for (std::size_t i = 0; i < nativeSize_; ++i)
        add(nativeEvents_[i]);
    contract::Event edge;
    std::uint32_t buttonCount = 0;
    for (std::size_t i = 0; i < std::min<std::size_t>(64, frames) &&
                            buttons_.peek(edge, static_cast<std::uint32_t>(i));
         ++i) {
        ++buttonCount;
        edge.offset = i;
        add(edge);
    }
    // MIDI has real sample positions. Only the declared CCs are accepted; no
    // note allocation or MIDI echo. Stable insertion sort gives bounded ties.
    for (std::size_t i = 0; i < nativeMidiSize_; ++i)
        add(nativeMidi_[i]);
    std::size_t midiCount = 0;
    for (const auto metadata : midi) {
        if (++midiCount > contract::eventCapacity) {
            overflow = true;
            break;
        }
        if (metadata.numBytes != 3 || (metadata.data[0] & 0xf0) != 0xb0)
            continue;
        const auto cc = metadata.data[1];
        const auto value = metadata.data[2];
        // Reject malformed MIDI data bytes instead of normalizing outside [0,1].
        if (cc > 127 || value > 127 || metadata.samplePosition < 0 ||
            static_cast<std::size_t>(metadata.samplePosition) >= frames) {
            overflow = true;
            break;
        }
        contract::Id id;
        float normalized;
        if (cc == 20 || cc == 21) {
            id = cc == 20 ? contract::Id::Flux : contract::Id::Bias;
            normalized = static_cast<float>(value) / 127;
        } else if (cc == 22) {
            id = contract::Id::Drive;
            normalized = value >= 64 ? 1.0F : 0.0F;
        } else if (cc == 23) {
            id = contract::Id::Domain;
            normalized = static_cast<float>(static_cast<unsigned>(value) * 3U / 128U) / 2;
        } else
            continue;
        add({static_cast<std::size_t>(metadata.samplePosition), contract::EventKind::Parameter, id,
             normalized});
        if (overflow)
            break;
    }
    for (std::size_t i = 1; i < size; ++i) {
        const auto event = events[i];
        auto j = i;
        while (j > 0 && events[j - 1].offset > event.offset) {
            events[j] = events[j - 1];
            --j;
        }
        events[j] = event;
    }
    // Validate the complete merged batch before consuming any queued UI edge.
    for (std::size_t i = 0; i < size; ++i)
        if (events[i].kind == contract::EventKind::Parameter &&
            !contract::valid(events[i].id, events[i].value))
            overflow = true;
    if (overflow)
        size = 0;
    const auto accepted = processor_.process(
        getTotalNumInputChannels() == 0 ? nullptr : b.getReadPointer(0),
        getTotalNumInputChannels() == 0 ? nullptr : b.getReadPointer(1), b.getWritePointer(0),
        b.getWritePointer(1), frames, std::span(events).first(size));
    overflow |= !accepted;
    if (overflow)
        rejected_.fetch_add(1, std::memory_order_relaxed);
    if (!overflow)
        buttons_.consume(buttonCount);
    if (snapshotValid) {
        // Host targets have already been published by the format wrapper. Even
        // a rejected batch must acknowledge them, or the next scalar snapshot
        // would replay a partial rejected transaction at offset zero.
        cached_ = candidate;
        cached_.values[bypassIndex] = bypassTarget;
    }
    auto status = processor_.status();
    status.rejected = overflow;
    const TelemetrySnapshot snapshot{++blockSequence_,
                                     static_cast<std::uint32_t>(frames),
                                     rejected_.load(std::memory_order_relaxed),
                                     droppedSnapshots_,
                                     sampleRate_,
                                     status};
    if (!telemetryQueue_.push(snapshot))
        ++droppedSnapshots_;
    clipped_.store(status.clipped ? 1U : 0U, std::memory_order_relaxed);
    input_.store(std::bit_cast<std::uint32_t>(status.inputPeak), std::memory_order_relaxed);
    output_.store(std::bit_cast<std::uint32_t>(status.outputPeak), std::memory_order_relaxed);
    const auto& t = status.engine;
    display_.store(t.word | (static_cast<std::uint32_t>(t.domain) << 8) |
                       (t.preview ? 1U << 10 : 0U) | (t.running ? 1U << 11 : 0U) |
                       (t.brake ? 1U << 12 : 0U),
                   std::memory_order_relaxed);
    midi.clear();
    beginNativeBlock(0);
}
void PluginProcessor::getStateInformation(juce::MemoryBlock& b) {
    std::lock_guard lock(stateMutex_);
    const auto bytes = contract::encode(controls());
    b.replaceAll(bytes.data(), bytes.size());
}
void PluginProcessor::setStateInformation(const void* data, int size) {
    if (!data || size < 0)
        return;
    contract::State state;
    if (contract::decode({static_cast<const std::uint8_t*>(data), static_cast<std::size_t>(size)},
                         state) == contract::DecodeResult::Ok)
        static_cast<void>(restore(state));
    else
        rejected_.fetch_add(1, std::memory_order_relaxed);
}
void PluginProcessor::setCurrentProgram(int index) {
    if (index >= 0 && index < getNumPrograms() &&
        restore(contract::presets[static_cast<std::size_t>(index)].state))
        program_.store(index);
}
const juce::String PluginProcessor::getProgramName(int index) {
    return index >= 0 && index < getNumPrograms()
               ? juce::String(contract::presets[static_cast<std::size_t>(index)].name.data())
               : juce::String{};
}
float PluginProcessor::inputPeak() const noexcept {
    return std::bit_cast<float>(input_.load(std::memory_order_relaxed));
}
float PluginProcessor::outputPeak() const noexcept {
    return std::bit_cast<float>(output_.load(std::memory_order_relaxed));
}
algorhythmia::Telemetry PluginProcessor::telemetry() const noexcept {
    const auto word = display_.load(std::memory_order_relaxed);
    return {static_cast<algorhythmia::SceneId>((word >> 8) & 3U), static_cast<std::uint8_t>(word),
            (word & (1U << 10)) != 0, (word & (1U << 11)) != 0, (word & (1U << 12)) != 0};
}
juce::AudioProcessorEditor* PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}
} // namespace sys01
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new sys01::PluginProcessor();
}
