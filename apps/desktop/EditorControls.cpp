#include "PluginEditor.hpp"
#include "Sys01Fonts.h"

namespace sys01::theme {
juce::Font mono(float height) {
    static const auto face = juce::Typeface::createSystemTypefaceFor(
        Sys01Fonts::jetbrainsmono_ttf, Sys01Fonts::jetbrainsmono_ttfSize);
    return juce::Font(juce::FontOptions(face).withHeight(height));
}
juce::Font body(float height) {
    static const auto face =
        juce::Typeface::createSystemTypefaceFor(Sys01Fonts::inter_ttf, Sys01Fonts::inter_ttfSize);
    return juce::Font(juce::FontOptions(face).withHeight(height));
}
juce::Font display(float height) {
    static const auto face = juce::Typeface::createSystemTypefaceFor(
        Sys01Fonts::barlowcondensed_ttf, Sys01Fonts::barlowcondensed_ttfSize);
    return juce::Font(juce::FontOptions(face).withHeight(height));
}
} // namespace sys01::theme

namespace sys01 {
namespace {
const juce::Colour bone(theme::bone), muted(theme::muted), dark(theme::basalt), pure(theme::pure),
    raised(theme::raised), border(theme::border), slate(theme::slate), obsidian(theme::obsidian);
void boundary(juce::Graphics& g, juce::Component& c) {
    g.setColour(c.hasKeyboardFocus(true) ? pure : border);
    g.drawRect(c.getLocalBounds().reduced(1), c.hasKeyboardFocus(true) ? 2 : 1);
}
} // namespace
NativeLook::NativeLook() {
    setColour(juce::Label::textColourId, bone);
    setColour(juce::ComboBox::backgroundColourId, dark);
    setColour(juce::ComboBox::textColourId, bone);
    setColour(juce::PopupMenu::backgroundColourId, dark);
    setColour(juce::PopupMenu::textColourId, bone);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, raised);
    setColour(juce::PopupMenu::highlightedTextColourId, pure);
    setColour(juce::TextEditor::backgroundColourId, dark);
    setColour(juce::TextEditor::textColourId, bone);
    setColour(juce::TextEditor::outlineColourId, muted);
    setColour(juce::TextEditor::focusedOutlineColourId, pure);
    setColour(juce::CaretComponent::caretColourId, pure);
}
void NativeLook::drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float pos, float,
                                  float, juce::Slider& slider) {
    auto r = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                                    static_cast<float>(w), static_cast<float>(h))
                 .reduced(14);
    auto c = r.getCentre();
    const auto radius = r.getWidth() / 2;
    const auto angle = (-5.F / 6 + 5.F / 3 * pos) * juce::MathConstants<float>::pi;
    const auto radial = [&](float a, float distance) {
        return c + juce::Point<float>(std::sin(a), -std::cos(a)) * distance;
    };
    // Six spline cuts, a recessed hub and sixteen quiet parameter apertures,
    // using the same proportions and 300-degree dial sweep as tape 01.
    for (int i = 0; i < 16; ++i) {
        const auto a = juce::MathConstants<float>::twoPi * static_cast<float>(i) / 16;
        const auto p = radial(a, radius + 7);
        const bool on = i <= juce::roundToInt(pos * 15);
        g.setColour(on ? bone : slate);
        if (on)
            g.fillEllipse(p.x - 1.2F, p.y - 1.2F, 2.4F, 2.4F);
        else
            g.drawEllipse(p.x - 1.2F, p.y - 1.2F, 2.4F, 2.4F, .75F);
    }
    g.setColour(raised);
    g.fillEllipse(r);
    g.setColour(slider.hasKeyboardFocus(false) ? pure : muted);
    g.drawEllipse(r, slider.hasKeyboardFocus(false) ? 2.F : 1.4F);
    const auto hub = r.withSizeKeepingCentre(r.getWidth() * .42F, r.getHeight() * .42F);
    g.setColour(obsidian);
    g.fillEllipse(hub);
    g.setColour(slate);
    g.drawEllipse(hub, 1);
    g.setColour(muted);
    for (int i = 0; i < 6; ++i) {
        const auto a = angle + juce::MathConstants<float>::twoPi * static_cast<float>(i) / 6;
        g.drawLine({radial(a, radius * .7F), radial(a, radius * .7F - 8)}, 1);
    }
    g.setColour(bone);
    g.drawLine({radial(angle, radius - 5), radial(angle, radius - 17)}, 3);
}
void NativeLook::drawLinearSlider(juce::Graphics& g, int x, int y, int w, int h, float pos, float,
                                  float, juce::Slider::SliderStyle, juce::Slider&) {
    g.setColour(slate);
    g.drawHorizontalLine(y + h / 2, static_cast<float>(x), static_cast<float>(x + w));
    g.setColour(bone);
    g.fillRect(pos - 1.5F, static_cast<float>(y + h / 2 - 7), 3.F, 14.F);
}
void NativeLook::drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&,
                                      bool over, bool down) {
    const bool toolbar = b.getComponentID().isNotEmpty();
    if (!toolbar || over || down)
        g.fillAll(over || down ? raised : dark);
    if (!toolbar || b.hasKeyboardFocus(true))
        boundary(g, b);
    if (down) {
        g.setColour(bone);
        g.drawHorizontalLine(b.getHeight() - 5, 5.F, static_cast<float>(b.getWidth() - 5));
    }
}
void NativeLook::drawButtonText(juce::Graphics& g, juce::TextButton& b, bool, bool) {
    juce::Graphics::ScopedSaveState save(g);
    if (b.getComponentID().isNotEmpty())
        g.addTransform(juce::AffineTransform::scale(b.getWidth() / 28.F, b.getHeight() / 28.F));
    g.setColour(b.isEnabled() ? bone : slate);
    const auto id = b.getComponentID();
    if (id == "settings") {
        for (int i = 0; i < 3; ++i) {
            const auto y = 8.F + i * 6;
            g.drawLine(6, y, 22, y, 1);
            g.fillRect(i == 1 ? 16.F : 9.F, y - 2, 3.F, 4.F);
        }
        return;
    }
    if (id == "undo" || id == "redo") {
        juce::Path arrow;
        arrow.startNewSubPath(7, 16);
        arrow.cubicTo(8, 7, 23, 7, 22, 20);
        arrow.startNewSubPath(6, 9);
        arrow.lineTo(6, 17);
        arrow.lineTo(14, 17);
        if (id == "redo")
            arrow.applyTransform(juce::AffineTransform::scale(-1, 1).translated(28, 0));
        g.strokePath(arrow, juce::PathStrokeType(1.2F));
        return;
    }
    g.setFont(theme::mono(12));
    g.drawText(b.getButtonText(), b.getLocalBounds().reduced(3), juce::Justification::centred);
}
void NativeLook::drawToggleButton(juce::Graphics& g, juce::ToggleButton& b, bool over, bool down) {
    const auto id = b.getComponentID();
    if (id == "drive" || id == "state") {
        const juce::Rectangle<float> cap((b.getWidth() - 42.F) / 2, 2, 42, 42);
        g.setColour(b.getToggleState() ? bone : over || down ? raised : obsidian);
        g.fillEllipse(cap);
        g.setColour(b.hasKeyboardFocus(true) ? pure : muted);
        g.drawEllipse(cap, b.hasKeyboardFocus(true) ? 2.F : 1.F);
        g.setColour(b.getToggleState() ? obsidian : bone);
        if (id == "state") {
            g.setFont(theme::mono(14));
            g.drawText(b.getToggleState() ? "1" : "0", cap, juce::Justification::centred);
        } else if (b.getToggleState()) {
            g.fillRect(cap.getCentreX() - 5, 16.F, 3.F, 14.F);
            g.fillRect(cap.getCentreX() + 2, 16.F, 3.F, 14.F);
        } else {
            juce::Path play;
            play.addTriangle(cap.getCentreX() - 4, 15, cap.getCentreX() - 4, 31,
                             cap.getCentreX() + 7, 23);
            g.fillPath(play);
        }
        g.setColour(bone);
        g.setFont(theme::mono(12));
        g.drawText(b.getButtonText(), 0, 52, b.getWidth(), 20, juce::Justification::centred);
        return;
    }
    drawButtonBackground(g, b, dark, over, down);
    g.setColour(bone);
    g.setFont(theme::mono(12));
    const bool domain = b.getRadioGroupId() != 0;
    g.drawText(b.getButtonText(), b.getLocalBounds().reduced(domain ? 12 : 4, 3),
               juce::Justification::centred);
    if (b.getToggleState()) {
        if (domain) {
            g.setColour(b.findColour(juce::ToggleButton::tickColourId));
            g.fillRect(6, b.getHeight() / 2 - 1, 3, 3);
        }
        g.setColour(bone);
        g.fillRect(5, b.getHeight() - 3, b.getWidth() - 10, 1);
    }
}
void NativeLook::drawComboBox(juce::Graphics& g, int w, int h, bool, int, int, int, int,
                              juce::ComboBox& c) {
    g.fillAll(dark);
    boundary(g, c);
    g.setColour(bone);
    g.drawLine(static_cast<float>(w - 16), h / 2.F - 2, static_cast<float>(w - 12), h / 2.F + 2, 1);
    g.drawLine(static_cast<float>(w - 12), h / 2.F + 2, static_cast<float>(w - 8), h / 2.F - 2, 1);
}
bool ActionButton::keyPressed(const juce::KeyPress& k) {
    if (k == juce::KeyPress::spaceKey || k == juce::KeyPress::returnKey) {
        if (!held_ && isEnabled()) {
            held_ = true;
            triggerClick();
        }
        return true;
    }
    return false;
}
bool ActionButton::keyStateChanged(bool down) {
    if (!down)
        held_ = false;
    return false;
}
bool TargetToggle::keyPressed(const juce::KeyPress& k) {
    if (adjacent && (k == juce::KeyPress::leftKey || k == juce::KeyPress::upKey ||
                     k == juce::KeyPress::rightKey || k == juce::KeyPress::downKey)) {
        adjacent(k == juce::KeyPress::leftKey || k == juce::KeyPress::upKey ? -1 : 1);
        return true;
    }
    if (k == juce::KeyPress::spaceKey || k == juce::KeyPress::returnKey) {
        if (!held_ && isEnabled()) {
            held_ = true;
            triggerClick();
        }
        return true;
    }
    return false;
}
bool TargetToggle::keyStateChanged(bool down) {
    if (!down)
        held_ = false;
    return false;
}
std::unique_ptr<juce::AccessibilityHandler> PerformanceSlider::createAccessibilityHandler() {
    class Value final : public juce::AccessibilityValueInterface {
    public:
        explicit Value(PerformanceSlider& s) : slider(s) {}
        bool isReadOnly() const override {
            return false;
        }
        double getCurrentValue() const override {
            return slider.getValue();
        }
        void setValue(double v) override {
            if (std::isfinite(v) && v >= 0 && v <= 1)
                slider.commit(v);
        }
        juce::String getCurrentValueAsString() const override {
            return slider.getTextFromValue(slider.getValue());
        }
        void setValueAsString(const juce::String& text) override {
            if (slider.parseText)
                if (const auto value = slider.parseText(text))
                    slider.commit(*value);
        }
        AccessibleValueRange getRange() const override {
            return {{0, 1}, .01};
        }

    private:
        PerformanceSlider& slider;
    };
    class Handler final : public juce::AccessibilityHandler {
    public:
        explicit Handler(PerformanceSlider& s)
            : AccessibilityHandler(s, juce::AccessibilityRole::slider, {},
                                   Interfaces{std::make_unique<Value>(s)}),
              slider(s) {}
        juce::String getHelp() const override {
            return slider.getTooltip();
        }

    private:
        PerformanceSlider& slider;
    };
    return std::make_unique<Handler>(*this);
}
void PerformanceSlider::beginGesture() {
    if (!gesture_)
        gesture_ = std::make_unique<ScopedDragNotification>(*this);
}
void PerformanceSlider::finishGesture() {
    gesture_.reset();
    heldKey_ = 0;
}
void PerformanceSlider::commit(double value) {
    finishGesture();
    ScopedDragNotification gesture(*this);
    setValue(value, juce::sendNotificationSync);
}
void PerformanceSlider::mouseDown(const juce::MouseEvent& e) {
    grabKeyboardFocus();
    lastY_ = e.position.y;
    if (e.getNumberOfClicks() == 1)
        beginGesture();
}
void PerformanceSlider::mouseDrag(const juce::MouseEvent& e) {
    if (!gesture_)
        return;
    setValue(getValue() + (lastY_ - e.position.y) / (e.mods.isShiftDown() ? 1800 : 180),
             juce::sendNotificationSync);
    lastY_ = e.position.y;
}
void PerformanceSlider::mouseUp(const juce::MouseEvent&) {
    finishGesture();
}
void PerformanceSlider::mouseDoubleClick(const juce::MouseEvent&) {
    commit(getDoubleClickReturnValue());
}
void PerformanceSlider::mouseWheelMove(const juce::MouseEvent& e,
                                       const juce::MouseWheelDetails& w) {
    if (hasKeyboardFocus(false) && w.deltaY != 0)
        commit(getValue() + (w.deltaY > 0 ? 1 : -1) * (e.mods.isShiftDown() ? .001 : .01));
}
bool PerformanceSlider::keyPressed(const juce::KeyPress& k) {
    const auto code = k.getKeyCode();
    if (code == juce::KeyPress::returnKey) {
        if (edit)
            edit();
        return true;
    }
    if (k.getModifiers().isCommandDown() || k.getModifiers().isAltDown() ||
        k.getModifiers().isCtrlDown())
        return false;
    const auto step = k.getModifiers().isShiftDown() ? .001 : .01;
    auto v = getValue();
    if (code == juce::KeyPress::upKey || code == juce::KeyPress::rightKey)
        v += step;
    else if (code == juce::KeyPress::downKey || code == juce::KeyPress::leftKey)
        v -= step;
    else if (code == juce::KeyPress::pageUpKey)
        v += .1;
    else if (code == juce::KeyPress::pageDownKey)
        v -= .1;
    else if (code == juce::KeyPress::homeKey)
        v = 0;
    else if (code == juce::KeyPress::endKey)
        v = 1;
    else
        return false;
    heldKey_ = code;
    beginGesture();
    setValue(v, juce::sendNotificationSync);
    return true;
}
bool PerformanceSlider::keyStateChanged(bool down) {
    if (!down && heldKey_ && !juce::KeyPress::isKeyCurrentlyDown(heldKey_)) {
        finishGesture();
        return true;
    }
    return false;
}
void PerformanceSlider::focusLost(FocusChangeType) {
    finishGesture();
    repaint();
}
void PerformanceSlider::paint(juce::Graphics& g) {
    juce::Slider::paint(g);
    if (hasKeyboardFocus(false) && getSliderStyle() != juce::Slider::RotaryVerticalDrag) {
        g.setColour(pure);
        g.drawRect(getLocalBounds().reduced(1), 2);
    }
}
ValueEntry::ValueEntry(PerformanceSlider& slider, contract::Id id) : slider_(slider), id_(id) {
    setName(juce::String(contract::catalogue[contract::index(id)].label.data()) +
            " numeric saved target");
    setTitle(getName());
    setFont(theme::mono(14));
    setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    setJustification(juce::Justification::centred);
    setSelectAllWhenFocused(true);
    onTextChange = [this] {
        if (editing_)
            dirty_ = true;
    };
    onReturnKey = [this] {
        if (accept())
            slider_.grabKeyboardFocus();
    };
    onEscapeKey = [this] {
        cancel();
        slider_.grabKeyboardFocus();
    };
    slider_.edit = [this] {
        grabKeyboardFocus();
        selectAll();
    };
}
void ValueEntry::refresh() {
    setText(view::format(id_, domain_, target_), false);
}
void ValueEntry::target(unsigned d, float v) {
    if (editing_ && id_ == contract::Id::Flux && d != domain_) {
        cancel();
        message_ = "DOMAIN CHANGED";
        setTooltip(message_);
        if (hasKeyboardFocus(false))
            slider_.grabKeyboardFocus();
    } else if (editing_ && v != target_) {
        message_ = "TARGET CHANGED; Enter writes your value; Escape uses latest target";
        setTooltip(message_);
    }
    domain_ = d;
    target_ = v;
    if (!editing_)
        refresh();
}
void ValueEntry::focusGained(FocusChangeType cause) {
    editing_ = true;
    dirty_ = false;
    editStart_ = getText();
    message_.clear();
    juce::TextEditor::focusGained(cause);
}
void ValueEntry::focusLost(FocusChangeType cause) {
    accept(true);
    juce::TextEditor::focusLost(cause);
}
void ValueEntry::cancel() {
    editing_ = dirty_ = false;
    message_.clear();
    setTooltip({});
    setDescription({});
    refresh();
}
bool ValueEntry::accept(bool focusLoss) {
    if (!editing_ || getText() == editStart_) {
        cancel();
        return true;
    }
    const auto p = view::parse(id_, domain_, getText());
    if (!p.value) {
        message_ = p.error;
        setTooltip(message_);
        setDescription(message_);
        if (focusLoss) {
            cancel();
            message_ = p.error;
            setTooltip(message_);
            setDescription(message_);
        } else {
            setColour(juce::TextEditor::focusedOutlineColourId, pure);
            repaint();
        }
        return false;
    }
    editing_ = dirty_ = false;
    target_ = *p.value;
    message_.clear();
    slider_.commit(*p.value);
    refresh();
    return true;
}
RegisterView::RegisterView() : TargetToggle("") {
    setTitle("Computation register brake");
    setClickingTogglesState(true);
    setWantsKeyboardFocus(true);
    setAccessible(true);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}
std::unique_ptr<juce::AccessibilityHandler> RegisterView::createAccessibilityHandler() {
    class Handler final : public juce::AccessibilityHandler {
    public:
        explicit Handler(RegisterView& r)
            : AccessibilityHandler(
                  r, juce::AccessibilityRole::toggleButton,
                  juce::AccessibilityActions()
                      .addAction(juce::AccessibilityActionType::press, [&r] { r.triggerClick(); })
                      .addAction(juce::AccessibilityActionType::toggle,
                                 [&r] {
                                     r.setToggleState(!r.getToggleState(),
                                                      juce::sendNotificationSync);
                                 })),
              r_(r) {}
        juce::AccessibleState getCurrentState() const override {
            const auto state = AccessibilityHandler::getCurrentState().withCheckable();
            return r_.getToggleState() ? state.withChecked() : state;
        }
        juce::String getHelp() const override {
            return "Click or press Space to toggle the brake.";
        }
        juce::String getDescription() const override {
            if (!r_.known)
                return "DSP register unavailable; " + r_.getDescription();
            juce::String bits;
            for (int i = 7; i >= 0; --i)
                bits += (r_.display.word & (1 << i)) ? "1" : "0";
            return "DSP register " + bits + ", hex " + juce::String::toHexString(r_.display.word) +
                   (r_.stale ? ", stale" : "") + "; " + r_.getDescription();
        }

    private:
        RegisterView& r_;
    };
    return std::make_unique<Handler>(*this);
}
void RegisterView::paint(juce::Graphics& g) {
    g.fillAll(obsidian);
    g.setColour(getToggleState() || hasKeyboardFocus(false) ? bone
                : isMouseOverOrDragging()                   ? muted
                                                            : slate);
    g.drawRect(getLocalBounds(), getToggleState() || hasKeyboardFocus(false) ? 2 : 1);
    g.setFont(theme::mono(12));
    g.setColour(muted);
    const auto caption = !known                                       ? "REGISTER"
                         : stale                                      ? "STALE"
                         : display.preview                            ? "PREVIEW"
                         : static_cast<unsigned>(display.domain) == 0 ? "WORD"
                         : static_cast<unsigned>(display.domain) == 1 ? "RULE"
                                                                      : "TAPE";
    g.drawText(caption, 0, 6, getWidth(), 18, juce::Justification::centred);
    for (int i = 0; i < 8; ++i) {
        const bool on = known && (display.word & (1 << (7 - i)));
        const juce::Rectangle<int> bit(15 + i * 12, 32, 6, 6);
        g.setColour(on && !stale ? juce::Colour(0xff000000U | display.color()) : dark);
        g.fillRect(bit);
        g.setColour(on ? muted : slate);
        g.drawRect(bit, 1);
    }
}
WindowScrew::WindowScrew(bool closesWindow)
    : Button(closesWindow ? "Close SYS 01" : "Minimise SYS 01"), closesWindow_(closesWindow) {
    setTitle(getName());
    setTooltip(closesWindow ? "Close" : "Minimise");
    setComponentID(closesWindow ? "window-close" : "window-minimise");
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setWantsKeyboardFocus(true);
}
void WindowScrew::place(juce::Point<float> point) {
    setBounds(juce::Rectangle<float>(point.x - 13, point.y - 13, 26, 26).toNearestInt());
    centre_ = point - getPosition().toFloat();
}
void WindowScrew::paintButton(juce::Graphics& g, bool over, bool down) {
    if (!over && !down && !hasKeyboardFocus(false))
        return; // The unchanged screw is already painted on the cassette.
    const juce::Rectangle<float> cap(centre_.x - 7, centre_.y - 7, 14, 14);
    g.setColour(down ? raised : obsidian);
    g.fillEllipse(cap);
    g.setColour(hasKeyboardFocus(false) ? pure : bone);
    g.drawEllipse(cap, hasKeyboardFocus(false) ? 2.F : 1.F);
    if (closesWindow_) {
        g.drawLine(centre_.x - 3, centre_.y - 3, centre_.x + 3, centre_.y + 3, 1);
        g.drawLine(centre_.x - 3, centre_.y + 3, centre_.x + 3, centre_.y - 3, 1);
    } else {
        g.drawLine(centre_.x - 3, centre_.y, centre_.x + 3, centre_.y, 1);
    }
}
Inspector::Inspector() {
    setName("SYS 01 details");
    setWantsKeyboardFocus(true);
    for (auto* c : std::array<juce::Component*, 3>{&close, &acknowledge, &body})
        addAndMakeVisible(c);
    body.setTitle("Mappings, saved targets and recovery");
    body.setMultiLine(true);
    body.setReadOnly(true);
    body.setScrollbarsShown(true);
    body.setFont(theme::body(14));
    acknowledge.setTooltip(
        "clear only the UI unresolved marker; no change to sound, counters or host targets");
    for (auto* c : std::array<juce::Component*, 5>{&audioSettings, &saveState, &loadState,
                                                   &resetState, &inputMuted})
        addChildComponent(c);
    inputMuted.setFont(theme::mono(11));
    inputMuted.setColour(juce::Label::textColourId, bone);
    inputMuted.setJustificationType(juce::Justification::centredRight);
    inputMuted.setTooltip(
        "Audio input is muted to avoid feedback. Open Audio / MIDI to change it.");
    resetState.setTooltip("Reset the standalone app to its default state; clears captured audio");
    saveState.setTooltip("Save current parameters; captured audio is not saved");
    loadState.setTooltip("Load saved parameters; clears captured audio");
}
void Inspector::setStandalone(bool enabled) {
    standalone_ = enabled;
    for (auto* c : std::array<juce::Component*, 5>{&audioSettings, &saveState, &loadState,
                                                   &resetState, &inputMuted})
        c->setVisible(enabled);
    resized();
}
bool Inspector::canModalEventBeSentToComponent(const juce::Component* c) {
    return standalone_ && c != nullptr &&
           (dynamic_cast<const WindowScrew*>(c) != nullptr ||
            dynamic_cast<const juce::ResizableBorderComponent*>(c) != nullptr);
}
std::unique_ptr<juce::AccessibilityHandler> Inspector::createAccessibilityHandler() {
    class Handler final : public juce::AccessibilityHandler {
    public:
        explicit Handler(Inspector& i)
            : AccessibilityHandler(i, juce::AccessibilityRole::dialogWindow), inspector(i) {}
        juce::String getDescription() const override {
            juce::String result = "Sampled stereo block peaks; not true peak. ";
            const std::array names{"input left", "input right", "output left", "output right"};
            for (std::size_t c = 0; c < 4; ++c)
                result +=
                    juce::String(names[c]) + " " +
                    (!inspector.health.available() || !inspector.health.meterValid ? "unavailable"
                     : inspector.health.meterDb[c] <= -60 && inspector.health.meterSilent[c]
                         ? "minus infinity DBFS"
                         : juce::String(inspector.health.meterDb[c], 1) + " DBFS") +
                    "; ";
            return result;
        }

    private:
        Inspector& inspector;
    };
    return std::make_unique<Handler>(*this);
}
bool Inspector::keyPressed(const juce::KeyPress& k) {
    if (k == juce::KeyPress::escapeKey) {
        close.triggerClick();
        return true;
    }
    return false;
}
void Inspector::resized() {
    close.setBounds(468, 8, 80, 28);
    body.setBounds(12, standalone_ ? 84 : 44, 536, standalone_ ? 110 : 150);
    audioSettings.setBounds(12, 44, 144, 28);
    saveState.setBounds(164, 44, 108, 28);
    loadState.setBounds(280, 44, 108, 28);
    resetState.setBounds(420, 44, 128, 28);
    inputMuted.setBounds(204, 8, 248, 28);
    acknowledge.setBounds(12, 260, 152, 28);
}
void Inspector::paint(juce::Graphics& g) {
    g.fillAll(dark);
    g.setColour(pure);
    g.drawRect(getLocalBounds(), 2);
    g.setFont(theme::display(22));
    g.drawText("SETTINGS", 12, 8, 440, 28, juce::Justification::left);
    constexpr std::array names{"IN L", "IN R", "OUT L", "OUT R"};
    for (int i = 0; i < 4; ++i) {
        const int x = 12 + i * 136;
        const auto db = health.meterDb[static_cast<std::size_t>(i)];
        g.setColour(bone);
        g.setFont(theme::mono(12));
        g.drawText(juce::String(names[static_cast<std::size_t>(i)]) + " " +
                       ((!health.available() || !health.meterValid) ? "--"
                        : db <= -60                                 ? "-INF"
                                                                    : juce::String(db, 1)) +
                       " DBFS",
                   x, 200, 130, 18, juce::Justification::left);
        g.setColour(muted);
        g.drawRect(x, 222, 124, 6);
        if (health.available() && health.meterValid) {
            g.setColour(bone);
            g.fillRect(x, 222, juce::roundToInt(124 * juce::jlimit(0.F, 1.F, (db + 60) / 60)), 6);
        }
    }
    g.setColour(muted);
    g.drawText("sampled block peaks / decay 20 DB per second / not true peak", 12, 236, 536, 18,
               juce::Justification::left);
    g.drawText("SCALE", 376, 260, 56, 28, juce::Justification::right);
}
} // namespace sys01
