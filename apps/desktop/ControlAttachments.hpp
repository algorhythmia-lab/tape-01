#pragma once
#include "PluginProcessor.hpp"

namespace sys01 {
// Editor-owned listeners retain JUCE's attach/detach synchronization. Off-message
// callbacks publish only a coalesced dirty bit, never a payload or OS message.
// Message-thread polling reads the authoritative publication word. Old outer
// notifications can request another read, but cannot replay an obsolete target.
class ControlAttachment {
public:
    ControlAttachment(juce::AudioProcessorValueTreeState& state, const char* key)
        : parameter_(*state.getParameter(key)), undo_(state.undoManager), relay_(*this) {
        parameter_.addListener(&relay_);
    }
    virtual ~ControlAttachment() {
        parameter_.removeListener(&relay_);
    }
    void poll() {
        // Clear before reading: a concurrent write after the read remains dirty
        // for the next poll. No sequence wrap, queue overflow or retained slot.
        relay_.dirty.exchange(false, std::memory_order_acq_rel);
        const auto publication = dynamic_cast<PublishedParameter&>(parameter_).capturePublication();
        if (!seen_ || publication != last_) {
            last_ = publication;
            seen_ = true;
            const juce::ScopedValueSetter<bool> scope(updating_, true);
            display(parameter_.convertFrom0to1(
                std::bit_cast<float>(static_cast<std::uint32_t>(publication))));
        }
    }

protected:
    juce::RangedAudioParameter& parameter_;
    bool updating_ = false; // message thread only
    void beginGesture() {
        if (undo_ != nullptr)
            undo_->beginNewTransaction();
        parameter_.beginChangeGesture();
    }
    void endGesture() {
        parameter_.endChangeGesture();
    }
    void write(float denormalised, bool complete) {
        if (updating_)
            return;
        const auto value = parameter_.convertTo0to1(denormalised);
        if (juce::approximatelyEqual(parameter_.getValue(), value))
            return;
        if (complete)
            beginGesture();
        parameter_.setValueNotifyingHost(value);
        if (complete)
            endGesture();
    }

private:
    juce::UndoManager* undo_;
    // A final relay has a stable vtable throughout derived-control destruction.
    // Other threads touch only this relay, which stays alive until removeListener
    // returns. They never dereference the editor/derived control. Message-thread
    // construction/destruction cannot overlap the synchronous owner callback.
    struct Relay final : juce::AudioProcessorParameter::Listener {
        explicit Relay(ControlAttachment& attachment)
            : owner(attachment), messageThread(juce::Thread::getCurrentThreadId()) {}
        ControlAttachment& owner;
        const juce::Thread::ThreadID messageThread;
        std::atomic<bool> dirty{true};
        void parameterValueChanged(int, float) override {
            dirty.store(true, std::memory_order_release);
            if (juce::Thread::getCurrentThreadId() == messageThread)
                owner.poll();
        }
        void parameterGestureChanged(int, bool) override {}
    } relay_;
    std::uint64_t last_ = 0;
    bool seen_ = false;
    virtual void display(float) = 0;
};
static_assert(std::atomic<bool>::is_always_lock_free);

class SliderAttachment final : public ControlAttachment, private juce::Slider::Listener {
public:
    SliderAttachment(juce::AudioProcessorValueTreeState& state, const char* key,
                     juce::Slider& slider)
        : ControlAttachment(state, key), slider_(slider) {
        // SYS 01's three continuous controls all have the native range [0, 1].
        const auto range = parameter_.getNormalisableRange();
        slider_.setRange(range.start, range.end, range.interval);
        slider_.setDoubleClickReturnValue(true,
                                          parameter_.convertFrom0to1(parameter_.getDefaultValue()));
        poll();
        slider_.valueChanged();
        slider_.addListener(this);
    }
    ~SliderAttachment() override {
        slider_.removeListener(this);
    }

private:
    juce::Slider& slider_;
    void display(float value) override {
        slider_.setValue(value, juce::sendNotificationSync);
    }
    void sliderValueChanged(juce::Slider*) override {
        write(static_cast<float>(slider_.getValue()), false);
    }
    void sliderDragStarted(juce::Slider*) override {
        beginGesture();
    }
    void sliderDragEnded(juce::Slider*) override {
        endGesture();
    }
};
class ButtonAttachment final : public ControlAttachment, private juce::Button::Listener {
public:
    ButtonAttachment(juce::AudioProcessorValueTreeState& state, const char* key,
                     juce::Button& button)
        : ControlAttachment(state, key), button_(button) {
        poll();
        button_.addListener(this);
    }
    ~ButtonAttachment() override {
        button_.removeListener(this);
    }

private:
    juce::Button& button_;
    void display(float value) override {
        button_.setToggleState(value >= .5F, juce::sendNotificationSync);
    }
    void buttonClicked(juce::Button*) override {
        write(button_.getToggleState() ? 1.F : 0.F, true);
    }
};
class ComboBoxAttachment final : public ControlAttachment, private juce::ComboBox::Listener {
public:
    ComboBoxAttachment(juce::AudioProcessorValueTreeState& state, const char* key,
                       juce::ComboBox& combo)
        : ControlAttachment(state, key), combo_(combo) {
        poll();
        combo_.addListener(this);
    }
    ~ComboBoxAttachment() override {
        combo_.removeListener(this);
    }

private:
    juce::ComboBox& combo_;
    void display(float value) override {
        const auto index = juce::roundToInt(parameter_.convertTo0to1(value) *
                                            static_cast<float>(combo_.getNumItems() - 1));
        if (index != combo_.getSelectedItemIndex())
            combo_.setSelectedItemIndex(index, juce::sendNotificationSync);
    }
    void comboBoxChanged(juce::ComboBox*) override {
        const auto count = combo_.getNumItems();
        const auto value = count > 1 ? static_cast<float>(combo_.getSelectedItemIndex()) /
                                           static_cast<float>(count - 1)
                                     : 0.F;
        write(parameter_.convertFrom0to1(value), true);
    }
};
} // namespace sys01
