#pragma once
#include "PluginEditor.hpp"

namespace juce {
class StandalonePluginHolder;
}
namespace sys01 {
class AudioSettingsTheme;
// The audio/device/state implementation remains JUCE's pinned standalone holder.
// Only the native window and access to its commands are replaced.
class StandaloneWindow final : public juce::ResizableWindow, private juce::Value::Listener {
public:
    explicit StandaloneWindow(std::unique_ptr<juce::StandalonePluginHolder>);
    ~StandaloneWindow() override;
    void saveState();
    void paint(juce::Graphics&) override {}
    void resized() override;
    void visibilityChanged() override;
    bool hitTest(int, int) override;
    bool keyPressed(const juce::KeyPress&) override;
    void userTriedToCloseWindow() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent&) override;
    juce::BorderSize<int> getBorderThickness() const override {
        return {};
    }
    juce::BorderSize<int> getContentComponentBorder() const override {
        return {};
    }

protected:
    int getDesktopWindowStyleFlags() const override;

private:
    class TapeResizeBorder;
    std::unique_ptr<juce::StandalonePluginHolder> holder_;
    std::unique_ptr<PluginEditor> editor_;
    std::unique_ptr<TapeResizeBorder> edge_;
    std::unique_ptr<AudioSettingsTheme> audioSettingsTheme_;
    juce::ComponentDragger dragger_;
    bool dragging_ = false;
    void attachEditor();
    void releaseEditor();
    void confirmReset();
    void showAudioSettings();
    void resetToDefaultState();
    void valueChanged(juce::Value&) override;
    friend class StandaloneFixture;
};
} // namespace sys01
