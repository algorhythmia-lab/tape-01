#include "StandaloneWindow.hpp"

#include "AudioSettingsTheme.hpp"
#include "TapeFace.hpp"

#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>
#include <juce_audio_utils/juce_audio_utils.h>

namespace sys01 {
namespace {
bool onTape(juce::Point<float> p, int width, int height) {
    return tape::outline().contains(
        p.transformedBy(view::canvasTransform(width, height).inverted()));
}
} // namespace
class StandaloneWindow::TapeResizeBorder final : public juce::ResizableBorderComponent {
public:
    TapeResizeBorder(StandaloneWindow& window, PluginEditor& editor)
        : ResizableBorderComponent(&window, editor.getConstrainer()), editor_(editor) {
        setAccessible(false);
    }
    void paint(juce::Graphics&) override {}
    bool hitTest(int x, int y) override {
        return ResizableBorderComponent::hitTest(x, y) &&
               onTape((juce::Point<int>(x, y) + getPosition()).toFloat(), editor_.getWidth(),
                      editor_.getHeight());
    }

private:
    PluginEditor& editor_;
};
StandaloneWindow::StandaloneWindow(std::unique_ptr<juce::StandalonePluginHolder> holder)
    : ResizableWindow("SYS 01", juce::Colours::transparentBlack, false),
      holder_(std::move(holder)) {
    setUsingNativeTitleBar(false);
    setDropShadowEnabled(false);
    setDraggable(false);
    setResizable(true, false);
    attachEditor();
    holder_->getMuteInputValue().addListener(this);
    valueChanged(holder_->getMuteInputValue());
    const auto* display = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    if (display != nullptr) {
        const auto area = display->userArea;
        auto x = area.getCentreX() - getWidth() / 2;
        auto y = area.getCentreY() - getHeight() / 2;
        if (auto* settings = holder_->settings.get()) {
            x = settings->getIntValue("windowX", x);
            y = settings->getIntValue("windowY", y);
        }
        const auto bounds = juce::Rectangle<int>(x, y, getWidth(), getHeight());
        const auto* target = juce::Desktop::getInstance().getDisplays().getDisplayForRect(bounds);
        const auto visible = target != nullptr ? target->userArea : area;
        setTopLeftPosition(
            juce::jlimit(visible.getX(),
                         juce::jmax(visible.getX(), visible.getRight() - getWidth()), x),
            juce::jlimit(visible.getY(),
                         juce::jmax(visible.getY(), visible.getBottom() - getHeight()), y));
    }
    addToDesktop();
}
StandaloneWindow::~StandaloneWindow() {
    audioSettingsTheme_ = nullptr;
    holder_->getMuteInputValue().removeListener(this);
    if (auto* settings = holder_->settings.get()) {
        settings->setValue("windowX", getX());
        settings->setValue("windowY", getY());
    }
    holder_->stopPlaying();
    releaseEditor();
}
int StandaloneWindow::getDesktopWindowStyleFlags() const {
    return ResizableWindow::getDesktopWindowStyleFlags() |
           juce::ComponentPeer::windowHasMinimiseButton | juce::ComponentPeer::windowHasCloseButton;
}
void StandaloneWindow::attachEditor() {
    editor_.reset(dynamic_cast<PluginEditor*>(holder_->processor->createEditorIfNeeded()));
    jassert(editor_ != nullptr);
    const juce::Component::SafePointer<StandaloneWindow> safe(this);
    editor_->setStandaloneActions({[safe] {
                                       if (safe != nullptr)
                                           safe->setMinimised(true);
                                   },
                                   [safe] {
                                       if (safe != nullptr)
                                           safe->userTriedToCloseWindow();
                                   },
                                   [safe] {
                                       if (safe != nullptr)
                                           safe->showAudioSettings();
                                   },
                                   [safe] {
                                       if (safe != nullptr)
                                           safe->holder_->askUserToSaveState();
                                   },
                                   [safe] {
                                       if (safe != nullptr)
                                           safe->holder_->askUserToLoadState();
                                   },
                                   [safe] {
                                       if (safe != nullptr)
                                           safe->confirmReset();
                                   }});
    editor_->getConstrainer()->setMinimumOnscreenAmounts(48, 48, 48, 48);
    setConstrainer(editor_->getConstrainer());
    edge_ = std::make_unique<TapeResizeBorder>(*this, *editor_);
    editor_->addAndMakeVisible(*edge_);
    editor_->addMouseListener(this, true);
    setContentNonOwned(editor_.get(), true);
    valueChanged(holder_->getMuteInputValue());
}
void StandaloneWindow::releaseEditor() {
    setConstrainer(nullptr);
    edge_ = nullptr;
    if (editor_ != nullptr) {
        editor_->removeMouseListener(this);
        clearContentComponent();
        holder_->processor->editorBeingDeleted(editor_.get());
        editor_ = nullptr;
    }
}
void StandaloneWindow::resized() {
    ResizableWindow::resized();
    if (edge_ != nullptr) {
        const auto transform = view::canvasTransform(editor_->getWidth(), editor_->getHeight());
        edge_->setBounds(
            tape::outline().getBoundsTransformed(transform).getSmallestIntegerContainer());
        edge_->setBorderThickness(juce::BorderSize<int>(juce::roundToInt(5 * transform.mat00)));
    }
}
bool StandaloneWindow::hitTest(int x, int y) {
    return onTape({float(x), float(y)}, getWidth(), getHeight());
}
void StandaloneWindow::visibilityChanged() {
    ResizableWindow::visibilityChanged();
    if (editor_ != nullptr)
        editor_->visibilityChanged();
}
void StandaloneWindow::mouseDown(const juce::MouseEvent& e) {
    dragging_ = e.mods.isLeftButtonDown() && editor_ != nullptr &&
                editor_->isWindowDragPoint(e.getEventRelativeTo(editor_.get()).getPosition());
    if (dragging_)
        dragger_.startDraggingComponent(this, e);
}
void StandaloneWindow::mouseDrag(const juce::MouseEvent& e) {
    if (dragging_)
        dragger_.dragComponent(this, e, getConstrainer());
}
void StandaloneWindow::mouseUp(const juce::MouseEvent&) {
    dragging_ = false;
}
bool StandaloneWindow::keyPressed(const juce::KeyPress& key) {
    if (!key.getModifiers().isCommandDown())
        return false;
    switch (juce::CharacterFunctions::toUpperCase(juce::juce_wchar(key.getKeyCode()))) {
    case 'M':
        setMinimised(true);
        return true;
    case 'W':
    case 'Q':
        userTriedToCloseWindow();
        return true;
    case ',':
        editor_->showSettings();
        return true;
    default:
        return false;
    }
}
void StandaloneWindow::saveState() {
    holder_->savePluginState();
    holder_->saveAudioDeviceState();
}
void StandaloneWindow::showAudioSettings() {
    audioSettingsTheme_ = nullptr;
    holder_->showAudioSettingsDialog();
    if (auto* dialog =
            dynamic_cast<juce::DialogWindow*>(juce::Component::getCurrentlyModalComponent()))
        audioSettingsTheme_ = std::make_unique<AudioSettingsTheme>(*dialog);
}
void StandaloneWindow::userTriedToCloseWindow() {
    if (auto* app = juce::JUCEApplication::getInstance())
        app->systemRequestedQuit();
}
void StandaloneWindow::valueChanged(juce::Value& value) {
    if (editor_ != nullptr)
        editor_->setStandaloneInputMuted(holder_->getProcessorHasPotentialFeedbackLoop() &&
                                         bool(value.getValue()));
}
void StandaloneWindow::confirmReset() {
    const juce::Component::SafePointer<StandaloneWindow> safe(this);
    juce::AlertWindow::showAsync(juce::MessageBoxOptions()
                                     .withIconType(juce::MessageBoxIconType::QuestionIcon)
                                     .withTitle("Reset app?")
                                     .withMessage("Restore defaults and clear captured audio?")
                                     .withButton("Reset")
                                     .withButton("Cancel")
                                     .withAssociatedComponent(editor_.get()),
                                 [safe](int result) {
                                     if (result == 1)
                                         juce::MessageManager::callAsync([safe] {
                                             if (safe != nullptr)
                                                 safe->resetToDefaultState();
                                         });
                                 });
}
void StandaloneWindow::resetToDefaultState() {
    const auto bounds = getBounds();
    holder_->stopPlaying();
    releaseEditor();
    holder_->deletePlugin();
    if (auto* settings = holder_->settings.get())
        settings->removeValue("filterState");
    holder_->createPlugin();
    attachEditor();
    setBounds(bounds);
    holder_->startPlaying();
    editor_->showSettings();
}
} // namespace sys01
