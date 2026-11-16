#include "StandaloneWindow.hpp"

#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>
#include <juce_audio_utils/juce_audio_utils.h>

namespace sys01 {
class StandaloneApp final : public juce::JUCEApplication {
public:
    StandaloneApp() {
        juce::PropertiesFile::Options options;
        options.applicationName = JucePlugin_Name;
        options.filenameSuffix = ".settings";
        options.osxLibrarySubFolder = "Application Support";
#if JUCE_LINUX || JUCE_BSD
        options.folderName = "~/.config";
#endif
        properties_.setStorageParameters(options);
    }
    const juce::String getApplicationName() override {
        return JucePlugin_Name;
    }
    const juce::String getApplicationVersion() override {
        return JucePlugin_VersionString;
    }
    bool moreThanOneInstanceAllowed() override {
        return true;
    }
    void anotherInstanceStarted(const juce::String&) override {}
    void initialise(const juce::String&) override {
        auto holder =
            std::make_unique<juce::StandalonePluginHolder>(properties_.getUserSettings(), false);
        if (juce::Desktop::getInstance().getDisplays().displays.isEmpty()) {
            headless_ = std::move(holder);
        } else {
            window_ = std::make_unique<StandaloneWindow>(std::move(holder));
            window_->setVisible(true);
            window_->toFront(true);
        }
    }
    void shutdown() override {
        headless_ = nullptr;
        window_ = nullptr;
        properties_.saveIfNeeded();
    }
    void systemRequestedQuit() override {
        if (headless_ != nullptr) {
            headless_->savePluginState();
            headless_->saveAudioDeviceState();
        }
        if (window_ != nullptr)
            window_->saveState();
        if (juce::ModalComponentManager::getInstance()->cancelAllModalComponents()) {
            juce::Timer::callAfterDelay(100, [] {
                if (auto* app = juce::JUCEApplicationBase::getInstance())
                    app->systemRequestedQuit();
            });
        } else {
            quit();
        }
    }

private:
    juce::ApplicationProperties properties_;
    std::unique_ptr<StandaloneWindow> window_;
    std::unique_ptr<juce::StandalonePluginHolder> headless_;
};
} // namespace sys01
// JUCE's standalone client still supplies the platform entry point.
JUCE_CREATE_APPLICATION_DEFINE(sys01::StandaloneApp)
