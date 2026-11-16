#include "StandaloneWindow.hpp"
#include "TapeFace.hpp"

#include <cstdio>
#if JUCE_MAC
bool sys01HasNativeCassetteWindow(void*);
#endif
#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>
#include <juce_audio_utils/juce_audio_utils.h>

namespace sys01 {
namespace {
void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
void image(juce::Component& component, const juce::File& path) {
    juce::FileOutputStream stream(path);
    require(stream.openedOk(), "could not open screenshot output");
    require(juce::PNGImageFormat().writeImageToStream(
                component.createComponentSnapshot(component.getLocalBounds()), stream),
            "could not render screenshot");
}
bool waitFor(const std::function<bool()>& condition) {
    const auto deadline = juce::Time::getMillisecondCounterHiRes() + 2000;
    while (!condition() && juce::Time::getMillisecondCounterHiRes() < deadline)
        juce::MessageManager::getInstance()->runDispatchLoopUntil(20);
    return condition();
}
} // namespace
class StandaloneFixture {
public:
    static void controls(const juce::File& folder) {
        PluginProcessor processor;
        PluginEditor editor(processor);
        editor.setVisible(true);
        std::array<int, 6> calls{};
        const StandaloneActions actions{[&] { ++calls[0]; }, [&] { ++calls[1]; },
                                        [&] { ++calls[2]; }, [&] { ++calls[3]; },
                                        [&] { ++calls[4]; }, [&] { ++calls[5]; }};
        for (const float scale : {1.F, 1.25F, 1.5F, 2.F}) {
            editor.setStandaloneActions({});
            editor.setSize(juce::roundToInt(view::canvasWidth * scale),
                           juce::roundToInt(view::canvasHeight * scale));
            const auto plugin = editor.createComponentSnapshot(editor.getLocalBounds());
            editor.setStandaloneActions(actions);
            const auto standalone = editor.createComponentSnapshot(editor.getLocalBounds());
            require(standalone.getPixelAt(0, 0).getAlpha() == 0 &&
                        standalone.getPixelAt(standalone.getWidth() - 1, standalone.getHeight() - 1)
                                .getAlpha() == 0,
                    "standalone still paints a rectangular background");
            require(plugin.getPixelAt(0, 0).getAlpha() == 255,
                    "DAW editor unexpectedly became transparent");
            for (int y = 0; y < standalone.getHeight(); ++y)
                for (int x = 0; x < standalone.getWidth(); ++x) {
                    const auto p = juce::Point<float>(float(x) / scale, float(y) / scale);
                    bool inside = true;
                    for (const auto d :
                         std::array<juce::Point<float>, 4>{{{-2, 0}, {2, 0}, {0, -2}, {0, 2}}})
                        inside = inside && tape::outline().contains(p + d);
                    if (inside)
                        require(plugin.getPixelAt(x, y) == standalone.getPixelAt(x, y),
                                "cassette face changed when enabling standalone controls");
                }
            image(editor, folder.getChildFile("tape-" + juce::String(int(scale * 100)) + ".png"));
            editor.showSettings();
            require(!editor.minimise_.isCurrentlyBlockedByAnotherModalComponent() &&
                        !editor.close_.isCurrentlyBlockedByAnotherModalComponent(),
                    "settings blocks window screws");
            require(editor.inspector_.audioSettings.isVisible() &&
                        editor.inspector_.saveState.isVisible() &&
                        editor.inspector_.loadState.isVisible() &&
                        editor.inspector_.resetState.isVisible(),
                    "standalone options missing from settings");
            image(editor,
                  folder.getChildFile("settings-" + juce::String(int(scale * 100)) + ".png"));
            editor.closeInspector();
        }
        const auto state = processor.controls();
        editor.minimise_.onClick();
        editor.close_.onClick();
        editor.inspector_.audioSettings.onClick();
        editor.inspector_.saveState.onClick();
        editor.inspector_.loadState.onClick();
        editor.inspector_.resetState.onClick();
        require(calls == std::array{1, 1, 1, 1, 1, 1}, "standalone command routing failed");
        require(processor.controls() == state, "window commands modified DSP parameters");
        editor.addToDesktop(0);
        const auto* minimiseHandler = editor.minimise_.getAccessibilityHandler();
        const auto* closeHandler = editor.close_.getAccessibilityHandler();
        require(minimiseHandler != nullptr && closeHandler != nullptr &&
                    minimiseHandler->getRole() == juce::AccessibilityRole::button &&
                    closeHandler->getTitle() == "Close SYS 01",
                "screw controls lack accessible button semantics");
        editor.removeFromDesktop();
        editor.setSize(view::canvasWidth, view::canvasHeight);
        require(editor.isWindowDragPoint({220, 110}) && !editor.isWindowDragPoint({192, 200}) &&
                    !editor.isWindowDragPoint({320, 40}) && !editor.isWindowDragPoint({43, 40}) &&
                    !editor.isWindowDragPoint({0, 0}),
                "window drag region overlaps a control or transparent margin");
        editor.setStandaloneInputMuted(true);
        require(editor.inspector_.inputMuted.getText() == "INPUT MUTED" &&
                    editor.inspector_.inputMuted.isVisible(),
                "input mute status missing");
        editor.setStandaloneInputMuted(false);
        require(!editor.inspector_.inputMuted.isVisible(),
                "unmuted input still announces a warning");
        editor.setStandaloneActions({});
        require(!editor.minimise_.isVisible() && !editor.close_.isVisible() &&
                    !editor.inspector_.audioSettings.isVisible() &&
                    editor.inspector_.body.getBounds() == juce::Rectangle<int>(12, 44, 536, 150),
                "standalone options leaked into the DAW editor");
        std::puts("standalone transparency, unchanged cassette pixels at four scales, command "
                  "routing, accessible screws and DAW isolation PASS");
    }
    static void lifecycle() {
        juce::PropertySet settings;
        settings.setValue("shouldMuteInput", true);
        auto expected = contract::presets[2].state;
        {
            auto holder = std::make_unique<juce::StandalonePluginHolder>(&settings, false);
            holder->deviceManager.closeAudioDevice();
            holder->stopPlaying();
            StandaloneWindow window(std::move(holder));
            require(!window.isOpaque() && !window.isUsingNativeTitleBar() &&
                        !window.isDropShadowEnabled(),
                    "native shell is not transparent and frameless");
            require((window.getPeer()->getStyleFlags() & juce::ComponentPeer::windowHasTitleBar) ==
                        0,
                    "native peer still has a title bar");
            auto& processor = *static_cast<PluginProcessor*>(window.holder_->processor.get());
            require(processor.restore(expected), "could not set lifecycle state");
            window.setSize(960, 624);
            require(window.editor_->getLocalBounds() == window.getLocalBounds(),
                    "shell and editor sizes differ");
            window.editor_->scale_.setSelectedId(4, juce::sendNotificationSync);
            require(window.getWidth() == 1280 && window.getHeight() == 832,
                    "interface scaling failed to resize the shell");
            window.saveState();
            require(settings.getValue("filterState").isNotEmpty() &&
                        settings.getBoolValue("shouldMuteInput"),
                    "save lost plugin state or changed input mute behavior");
            require(
                window.keyPressed(juce::KeyPress(',', juce::ModifierKeys::commandModifier, ',')) &&
                    window.editor_->inspector_.isVisible(),
                "settings shortcut failed");
            window.editor_->closeInspector();
            window.setVisible(true);
            window.toFront(true);
            juce::MessageManager::getInstance()->runDispatchLoopUntil(50);
            window.holder_->startPlaying();
            window.editor_->showSettings();
#if JUCE_MAC
            require(sys01HasNativeCassetteWindow(window.getWindowHandle()),
                    "Cocoa window decorations or transparency differ from the cassette shell");
#endif
            window.editor_->minimise_.onClick();
            require(waitFor([&] { return window.isMinimised(); }) &&
                        window.holder_->player.getCurrentProcessor() == &processor &&
                        processor.controls() == expected,
                    "minimise failed or changed audio playback/parameters");
            window.setMinimised(false);
            require(waitFor([&] { return !window.isMinimised(); }),
                    "cassette window did not restore");
            require(waitFor([&] { return window.editor_->isTimerRunning(); }),
                    "restored cassette did not resume telemetry");
            window.editor_->closeInspector();
            require(
                window.keyPressed(juce::KeyPress('m', juce::ModifierKeys::commandModifier, 'm')) &&
                    waitFor([&] { return window.isMinimised(); }),
                "minimise shortcut failed");
            window.setMinimised(false);
            window.setVisible(false);
        }
        {
            auto holder = std::make_unique<juce::StandalonePluginHolder>(&settings, false);
            holder->deviceManager.closeAudioDevice();
            holder->stopPlaying();
            StandaloneWindow window(std::move(holder));
            require(static_cast<PluginProcessor*>(window.holder_->processor.get())->controls() ==
                        expected,
                    "reopening standalone did not restore saved targets");
            const auto bounds = window.getBounds();
            window.resetToDefaultState();
            window.holder_->stopPlaying();
            require(static_cast<PluginProcessor*>(window.holder_->processor.get())->controls() ==
                            contract::State{} &&
                        window.getBounds() == bounds && window.editor_->inspector_.isVisible(),
                    "standalone reset failed to recreate the default processor and settings");
            require(window.holder_->getMuteInputValue().getValue(),
                    "reset unexpectedly unmuted input");
        }
        std::puts("native frameless peer, scale propagation, minimize/restore with audio attached, "
                  "saved state reopen, input mute preservation and full app reset PASS");
    }
};
} // namespace sys01
class StandaloneFixtureApp final : public juce::JUCEApplication {
public:
    const juce::String getApplicationName() override {
        return "SYS 01 window fixture";
    }
    const juce::String getApplicationVersion() override {
        return "1";
    }
    void initialise(const juce::String&) override {
        const auto args = getCommandLineParameterArray();
        if (args.size() != 1) {
            setApplicationReturnValue(1);
            quit();
            return;
        }
        const juce::File folder(args[0]);
        if (folder.createDirectory().failed()) {
            setApplicationReturnValue(1);
            quit();
            return;
        }
        juce::MessageManager::callAsync([folder] {
            sys01::StandaloneFixture::controls(folder);
            sys01::StandaloneFixture::lifecycle();
            quit();
        });
    }
    void shutdown() override {}
};
START_JUCE_APPLICATION(StandaloneFixtureApp)
