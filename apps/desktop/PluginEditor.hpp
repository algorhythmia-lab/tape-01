#pragma once
#include "ControlAttachments.hpp"
#include "EditorValues.hpp"
#include "Theme.hpp"
#include "Typography.hpp"
namespace sys01 {
class NativeLook final : public juce::LookAndFeel_V4 {
public:
    NativeLook();
    void drawRotarySlider(juce::Graphics&, int, int, int, int, float, float, float,
                          juce::Slider&) override;
    void drawLinearSlider(juce::Graphics&, int, int, int, int, float, float, float,
                          juce::Slider::SliderStyle, juce::Slider&) override;
    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour&, bool,
                              bool) override;
    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) override;
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool, bool) override;
    void drawComboBox(juce::Graphics&, int, int, bool, int, int, int, int,
                      juce::ComboBox&) override;
    juce::Font getComboBoxFont(juce::ComboBox&) override {
        return theme::mono(12);
    }
    juce::Font getPopupMenuFont() override {
        return theme::mono(13);
    }
};
class PerformanceSlider final : public juce::Slider {
public:
    ~PerformanceSlider() override {
        finishGesture();
    }
    std::function<void()> edit, beforeGesture, afterGesture;
    std::function<std::optional<double>(const juce::String&)> parseText;
    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override;
    void startedDragging() override {
        if (beforeGesture)
            beforeGesture();
    }
    void stoppedDragging() override {
        if (afterGesture)
            afterGesture();
    }
    void commit(double value);
    void finishGesture();
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent&) override;
    void mouseDoubleClick(const juce::MouseEvent&) override;
    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;
    bool keyPressed(const juce::KeyPress&) override;
    bool keyStateChanged(bool) override;
    void focusLost(FocusChangeType) override;
    void paint(juce::Graphics&) override;

private:
    std::unique_ptr<ScopedDragNotification> gesture_;
    float lastY_ = 0;
    int heldKey_ = 0;
    void beginGesture();
};
class ActionButton final : public juce::TextButton {
public:
    using juce::TextButton::TextButton;
    bool keyPressed(const juce::KeyPress&) override;
    bool keyStateChanged(bool) override;
    void focusLost(FocusChangeType) override {
        held_ = false;
        repaint();
    }

private:
    bool held_ = false;
};
class TargetToggle : public juce::ToggleButton {
public:
    using juce::ToggleButton::ToggleButton;
    std::function<void()> beforeClick;
    std::function<void(int)> adjacent;
    bool keyPressed(const juce::KeyPress&) override;
    bool keyStateChanged(bool) override;
    void focusLost(FocusChangeType) override {
        held_ = false;
        repaint();
    }
    void clicked() override {
        if (beforeClick)
            beforeClick();
    }

private:
    bool held_ = false;
};
class ValueEntry final : public juce::TextEditor {
public:
    ValueEntry(PerformanceSlider&, contract::Id);
    void target(unsigned domain, float value);
    void cancel();
    bool accept(bool focusLoss = false);
    void focusGained(FocusChangeType) override;
    void focusLost(FocusChangeType) override;
    juce::String message() const {
        return message_;
    }

private:
    PerformanceSlider& slider_;
    contract::Id id_;
    unsigned domain_ = 0;
    float target_ = 0;
    bool editing_ = false, dirty_ = false;
    juce::String message_, editStart_;
    void refresh();
};
class RegisterView final : public TargetToggle {
public:
    algorhythmia::Telemetry display{};
    bool known = false, stale = false;
    RegisterView();
    void paint(juce::Graphics&) override;
    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override;
};
// Supplied only by the standalone shell. DAW editors never receive these actions.
struct StandaloneActions {
    std::function<void()> minimise, close, audioSettings, saveState, loadState, resetState;
};
class WindowScrew final : public juce::Button {
public:
    explicit WindowScrew(bool closesWindow);
    void place(juce::Point<float>);
    void paintButton(juce::Graphics&, bool, bool) override;

private:
    bool closesWindow_;
    juce::Point<float> centre_;
};
class Inspector final : public juce::Component {
public:
    Inspector();
    ActionButton close{"CLOSE"}, acknowledge{"ACKNOWLEDGE"};
    ActionButton audioSettings{"AUDIO / MIDI"}, saveState{"SAVE STATE"}, loadState{"LOAD STATE"},
        resetState{"RESET APP..."};
    juce::Label inputMuted;
    juce::TextEditor body;
    view::Health health;
    void setStandalone(bool);
    bool canModalEventBeSentToComponent(const juce::Component*) override;
    bool keyPressed(const juce::KeyPress&) override;
    void paint(juce::Graphics&) override;
    void resized() override;
    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override;

private:
    bool standalone_ = false;
};
class PluginEditor final : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress&) override;
    void visibilityChanged() override;
    void parentHierarchyChanged() override;
    void setStandaloneActions(StandaloneActions);
    void setStandaloneInputMuted(bool);
    void showSettings();
    bool hitTest(int, int) override;
    bool isWindowDragPoint(juce::Point<int>);

private:
    PluginProcessor& processor_;
    NativeLook look_;
    juce::Component face_;
    PerformanceSlider flux_, bias_, output_;
    ValueEntry fluxEntry_{flux_, contract::Id::Flux}, biasEntry_{bias_, contract::Id::Bias},
        outputEntry_{output_, contract::Id::Output};
    TargetToggle drive_{"DRIVE"}, state_{"STATE"}, bypass_{"BYPASS"};
    juce::ComboBox domain_, preset_, scale_;
    std::array<TargetToggle, 3> domains_;
    ActionButton undo_{"UNDO"}, redo_{"REDO"}, reset_{"RESET"}, details_{"SETTINGS"};
    juce::Label fluxLabel_, biasLabel_, outputLabel_, context_, input_, limiting_, rejection_,
        health_, recall_, validation_;
    RegisterView register_;
    Inspector inspector_;
    StandaloneActions standalone_;
    WindowScrew minimise_{false}, close_{true};
    juce::TooltipWindow tooltips_{this, 500};
    std::unique_ptr<SliderAttachment> fluxAttachment_, biasAttachment_, outputAttachment_;
    std::unique_ptr<ButtonAttachment> driveAttachment_, stateAttachment_, bypassAttachment_;
    std::unique_ptr<ComboBoxAttachment> domainAttachment_;
    TelemetrySnapshot snapshot_{};
    view::Health view_;
    bool brakePending_ = false, syncingScale_ = false, editorActive_ = false;
    void syncVisibility();
    juce::String announcedFacts_;
    double lastAnnouncement_ = 0;
    void timerCallback() override;
    void updateView(const TelemetrySnapshot&, double now);
    void closeInspector();
    juce::String inspectorText(const contract::State&) const;
    friend class EditorFixture;
    friend class StandaloneFixture;
};
} // namespace sys01
