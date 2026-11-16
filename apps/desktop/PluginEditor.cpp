#include "PluginEditor.hpp"

#include "TapeFace.hpp"
namespace sys01 {
namespace {
void label(juce::Label& l, const juce::String& text, float size = 12) {
    l.setText(text, juce::dontSendNotification);
    l.setFont(theme::mono(size));
    l.setBorderSize({});
    l.setColour(juce::Label::textColourId, juce::Colour(theme::muted));
}
void text(juce::Label& l, const juce::String& t) {
    if (l.getText() != t)
        l.setText(t, juce::dontSendNotification);
}
} // namespace
PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(p), processor_(p) {
    setLookAndFeel(&look_);
    addAndMakeVisible(face_);
    face_.setBounds(0, 0, view::canvasWidth, view::canvasHeight);
    for (auto* c : std::array<juce::Component*, 28>{
             &flux_,       &bias_,     &output_,     &fluxEntry_, &biasEntry_, &outputEntry_,
             &drive_,      &state_,    &bypass_,     &preset_,    &scale_,     &undo_,
             &redo_,       &reset_,    &details_,    &fluxLabel_, &biasLabel_, &outputLabel_,
             &context_,    &input_,    &limiting_,   &rejection_, &health_,    &recall_,
             &validation_, &register_, &domains_[0], &domains_[1]})
        face_.addAndMakeVisible(c);
    face_.addAndMakeVisible(domains_[2]);
    face_.addChildComponent(inspector_);
    inspector_.addAndMakeVisible(reset_);
    inspector_.addAndMakeVisible(scale_);
    undo_.setComponentID("undo");
    redo_.setComponentID("redo");
    details_.setComponentID("settings");
    drive_.setComponentID("drive");
    state_.setComponentID("state");
    undo_.setTooltip("undo parameter edit");
    redo_.setTooltip("redo parameter edit");
    details_.setTooltip("settings, interface scale and diagnostics");
    details_.setTitle("SETTINGS");
    drive_.setTitle("DRIVE transport");
    state_.setTitle("STATE alternate register");
    bypass_.setTitle("BYPASS dry input");
    face_.addChildComponent(
        domain_); // Existing APVTS choice binding; visible native radio controls drive it.
    label(fluxLabel_, "FLUX", 14);
    label(biasLabel_, "BIAS", 14);
    label(outputLabel_, "OUTPUT", 12);
    fluxLabel_.setJustificationType(juce::Justification::centred);
    biasLabel_.setJustificationType(juce::Justification::centred);
    for (auto* l : {&context_, &input_, &limiting_, &rejection_, &health_, &recall_, &validation_})
        label(*l, "");
    context_.setJustificationType(juce::Justification::centred);
    context_.setInterceptsMouseClicks(false, false);
    context_.setAccessible(false);
    context_.toFront(false);
    validation_.setJustificationType(juce::Justification::centred);
    health_.setJustificationType(juce::Justification::right);
    rejection_.setJustificationType(juce::Justification::right);
    label(recall_, "parameter recall; reset clears audio");
    recall_.setVisible(false);
    health_.setVisible(false);
    limiting_.setColour(juce::Label::textColourId, juce::Colour(theme::bone));
    for (auto* s : {&flux_, &bias_, &output_}) {
        s->setSliderStyle(s == &output_ ? juce::Slider::LinearHorizontal
                                        : juce::Slider::RotaryVerticalDrag);
        s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        s->setRange(0, 1, 0);
        s->setWantsKeyboardFocus(true);
        s->setScrollWheelEnabled(false);
        s->setDoubleClickReturnValue(true, s == &output_ ? .75 : .5);
        s->beforeGesture = [this] { static_cast<void>(processor_.parameters.copyState()); };
        s->afterGesture = [this] { static_cast<void>(processor_.parameters.copyState()); };
        s->setTooltip("saved target; vertical drag; Shift fine; arrows step; Enter numeric entry; "
                      "double click default");
    }
    flux_.setName("FLUX saved target");
    bias_.setName("BIAS coupled macro saved target");
    output_.setName("OUTPUT wet trim saved target");
    flux_.setTitle(flux_.getName());
    bias_.setTitle(bias_.getName());
    output_.setTitle(output_.getName());
    domain_.addItemList({"00 BYTEBEAT", "01 CELLULAR", "02 TRANSFORM"}, 1);
    const std::array<juce::String, 3> domainNames{"00 BYTEBEAT", "01 CELLULAR", "02 TRANSFORM"};
    for (int i = 0; i < 3; ++i) {
        auto& b = domains_[static_cast<std::size_t>(i)];
        b.setTitle(domainNames[static_cast<std::size_t>(i)] + " domain");
        b.setButtonText(domainNames[static_cast<std::size_t>(i)]);
        b.setRadioGroupId(1);
        b.setClickingTogglesState(true);
        b.setColour(juce::ToggleButton::tickColourId, juce::Colour(i == 0   ? theme::amber
                                                                   : i == 1 ? theme::green
                                                                            : theme::cyan));
        b.adjacent = [this, i](int delta) {
            const auto next = (i + delta + 3) % 3;
            domains_[static_cast<std::size_t>(next)].grabKeyboardFocus();
            domains_[static_cast<std::size_t>(next)].triggerClick();
        };
        b.onClick = [this, i] {
            static_cast<void>(processor_.parameters.copyState());
            domain_.setSelectedId(i + 1, juce::sendNotificationSync);
            static_cast<void>(processor_.parameters.copyState());
        };
    }
    for (std::size_t i = 0; i < contract::presets.size(); ++i)
        preset_.addItem(contract::presets[i].name.data(), static_cast<int>(i + 1));
    preset_.setName("factory study; parameter-only recall clears audio");
    preset_.onChange = [this] {
        if (preset_.getSelectedId() > 0) {
            static_cast<void>(processor_.parameters.copyState());
            processor_.undo.beginNewTransaction("preset; parameters only");
            processor_.setCurrentProgram(preset_.getSelectedId() - 1);
            static_cast<void>(processor_.parameters.copyState());
        }
    };
    scale_.setTitle("interface scale");
    scale_.addItemList({"100%", "125%", "150%", "200%"}, 1);
    scale_.onChange = [this] {
        if (syncingScale_ || scale_.getSelectedId() < 1)
            return;
        constexpr std::array scales{1.F, 1.25F, 1.5F, 2.F};
        const auto s = scales[static_cast<std::size_t>(scale_.getSelectedId() - 1)];
        setSize(juce::roundToInt(view::canvasWidth * s), juce::roundToInt(view::canvasHeight * s));
    };
    undo_.onClick = [this] { processor_.undo.undo(); };
    redo_.onClick = [this] { processor_.undo.redo(); };
    reset_.onClick = [this] {
        static_cast<void>(processor_.parameters.copyState());
        processor_.undo.beginNewTransaction("reset; parameters only");
        static_cast<void>(processor_.restore(contract::State{}));
        static_cast<void>(processor_.parameters.copyState());
    };
    for (auto* c : std::array<juce::SettableTooltipClient*, 4>{&reset_, &preset_, &undo_, &redo_})
        c->setTooltip("parameter-only recall; reset/presets clear audio, voices and phase; undo "
                      "cannot recover captured audio");
    for (auto* toggle : {&drive_, &state_, &bypass_}) {
        toggle->beforeClick = [this] { static_cast<void>(processor_.parameters.copyState()); };
        toggle->onClick = [this] { static_cast<void>(processor_.parameters.copyState()); };
    }
    register_.onClick = [this] {
        if (!processor_.button(true, register_.getToggleState()))
            register_.setToggleState(!register_.getToggleState(), juce::dontSendNotification);
        else
            brakePending_ = true;
    };
    details_.onClick = [this] { showSettings(); };
    inspector_.close.onClick = [this] { closeInspector(); };
    inspector_.acknowledge.onClick = [this] {
        view_.unresolved = false;
        inspector_.body.setText(inspectorText(processor_.controls()), false);
    };
    fluxAttachment_ = std::make_unique<SliderAttachment>(p.parameters, "param_flux", flux_);
    biasAttachment_ = std::make_unique<SliderAttachment>(p.parameters, "param_bias", bias_);
    outputAttachment_ = std::make_unique<SliderAttachment>(p.parameters, "output_trim", output_);
    driveAttachment_ = std::make_unique<ButtonAttachment>(p.parameters, "transport_drive", drive_);
    stateAttachment_ =
        std::make_unique<ButtonAttachment>(p.parameters, "subregister_state", state_);
    bypassAttachment_ = std::make_unique<ButtonAttachment>(p.parameters, "bypass", bypass_);
    domainAttachment_ = std::make_unique<ComboBoxAttachment>(p.parameters, "engine_state", domain_);
    for (auto pair : {std::pair{&flux_, contract::Id::Flux},
                      {&bias_, contract::Id::Bias},
                      {&output_, contract::Id::Output}}) {
        pair.first->textFromValueFunction = [this, id = pair.second](double v) {
            return view::format(id, view::domain(processor_.controls()), static_cast<float>(v));
        };
        pair.first->parseText = [this,
                                 id = pair.second](const juce::String& t) -> std::optional<double> {
            const auto parsed = view::parse(id, view::domain(processor_.controls()), t);
            if (parsed.value)
                return *parsed.value;
            return {};
        };
        pair.first->valueFromTextFunction = [this, id = pair.second,
                                             s = pair.first](const juce::String& t) {
            return static_cast<double>(view::parse(id, view::domain(processor_.controls()), t)
                                           .value.value_or(static_cast<float>(s->getValue())));
        };
    }
    // Explicit focus traversal. Direct-entry fields are separate native edit actions.
    int order = 1;
    for (auto* c : std::array<juce::Component*, 19>{
             &details_, &preset_, &undo_, &redo_, &scale_, &domains_[0], &domains_[1], &domains_[2],
             &flux_, &fluxEntry_, &bias_, &biasEntry_, &drive_, &register_, &state_, &output_,
             &outputEntry_, &bypass_, &reset_})
        c->setExplicitFocusOrder(order++);
    setResizable(true, true);
    setResizeLimits(view::canvasWidth, view::canvasHeight, view::canvasWidth * 2,
                    view::canvasHeight * 2);
    setSize(view::canvasWidth, view::canvasHeight);
    updateView({}, juce::Time::getMillisecondCounterHiRes());
    syncVisibility();
}
PluginEditor::~PluginEditor() {
    stopTimer();
    if (inspector_.isCurrentlyModal())
        inspector_.exitModalState(0);
    for (auto* s : {&flux_, &bias_, &output_})
        s->finishGesture();
    if (register_.getToggleState() || snapshot_.audio.engine.brake)
        static_cast<void>(processor_.button(true, false));
    setLookAndFeel(nullptr);
}
void PluginEditor::resized() {
    const auto transform = view::canvasTransform(getWidth(), getHeight());
    face_.setTransform(transform);
    minimise_.place(tape::point(4, 4));
    close_.place(tape::point(96.4F, 4));
    preset_.setBounds(260, 28, 240, 24);
    undo_.setBounds(504, 28, 24, 24);
    redo_.setBounds(532, 28, 24, 24);
    details_.setBounds(560, 28, 24, 24);
    for (int i = 0; i < 3; ++i)
        domains_[static_cast<std::size_t>(i)].setBounds(140 + i * 120, 68, 120, 28);
    fluxLabel_.setBounds(120, 280, 144, 20);
    biasLabel_.setBounds(376, 280, 144, 20);
    flux_.setBounds(114, 129, 156, 156);
    bias_.setBounds(370, 129, 156, 156);
    fluxEntry_.setBounds(120, 302, 144, 24);
    biasEntry_.setBounds(376, 302, 144, 24);
    register_.setBounds(260, 170, 120, 74);
    context_.setBounds(260, 218, 120, 24);
    drive_.setBounds(66, 270, 80, 76);
    state_.setBounds(494, 270, 80, 76);
    outputLabel_.setBounds(164, 360, 48, 28);
    output_.setBounds(220, 360, 112, 28);
    outputEntry_.setBounds(340, 360, 80, 28);
    bypass_.setBounds(428, 360, 72, 28);
    input_.setBounds(164, 340, 144, 20);
    limiting_.setBounds(176, 116, 80, 18);
    rejection_.setBounds(360, 116, 200, 18);
    recall_.setBounds(24, 388, 304, 18);
    health_.setBounds(344, 388, 272, 18);
    validation_.setBounds(48, 98, 544, 18);
    inspector_.setBounds(40, 72, 560, 300);
    reset_.setBounds(180, 260, 88, 28);
    scale_.setBounds(444, 260, 104, 28);
    syncingScale_ = true;
    scale_.setSelectedId(0, juce::dontSendNotification);
    scale_.setText(juce::String(transform.mat00 * 100,
                                juce::approximatelyEqual(std::round(transform.mat00 * 100),
                                                         transform.mat00 * 100)
                                    ? 0
                                    : 1) +
                       "%",
                   juce::dontSendNotification);
    syncingScale_ = false;
}
void PluginEditor::paint(juce::Graphics& g) {
    if (!standalone_.close)
        g.fillAll(juce::Colour(theme::obsidian));
    g.addTransform(view::canvasTransform(getWidth(), getHeight()));
    g.setColour(juce::Colour(theme::basalt));
    g.fillPath(tape::outline());
    g.setColour(juce::Colour(theme::slate));
    g.strokePath(tape::outline(), juce::PathStrokeType(1));
    g.setColour(juce::Colour(theme::obsidian));
    g.fillPath(tape::head());
    g.setColour(juce::Colour(theme::slate));
    g.strokePath(tape::head(), juce::PathStrokeType(1));
    for (const auto position : std::array<juce::Point<float>, 5>{
             tape::point(4, 4), tape::point(96.4F, 4), tape::point(4, 59.8F),
             tape::point(96.4F, 59.8F), tape::point(50.2F, 54)}) {
        g.setColour(juce::Colour(theme::obsidian));
        g.fillEllipse(position.x - 7, position.y - 7, 14, 14);
        g.setColour(juce::Colour(theme::slate));
        g.drawEllipse(position.x - 7, position.y - 7, 14, 14, 1);
        g.drawLine(position.x - 3, position.y, position.x + 3, position.y);
        g.drawLine(position.x, position.y - 3, position.x, position.y + 3);
    }

    // Exact mark paths from TerminalHeader.tsx, including its filled core.
    static const auto mark = juce::Drawable::parseSVGPath(
        "M7 2v4m10-4v4M7 18v4m10-4v4M2 7h4m-4 10h4m12-10h4m-4 10h4M6 6h12v12H6z");
    static const auto core = juce::Drawable::parseSVGPath("M10 10h4v4h-4z");
    g.setColour(juce::Colour(theme::bone));
    const auto logoTransform = juce::AffineTransform::scale(22.F / 24).translated(58, 29);
    g.strokePath(
        mark, juce::PathStrokeType(1, juce::PathStrokeType::curved, juce::PathStrokeType::rounded),
        logoTransform);
    g.fillPath(core, logoTransform);
    g.setFont(theme::mono(17).withExtraKerningFactor(-.055F));
    g.drawText("algorhythmia", 88, 28, 104, 24, juce::Justification::left);
    g.setFont(theme::display(20));
    g.drawText("SYS 01", 204, 28, 48, 24, juce::Justification::left);
}
void PluginEditor::setStandaloneActions(StandaloneActions actions) {
    standalone_ = std::move(actions);
    const bool enabled = static_cast<bool>(standalone_.close);
    face_.addChildComponent(minimise_);
    face_.addChildComponent(close_);
    minimise_.setVisible(enabled);
    close_.setVisible(enabled);
    minimise_.onClick = standalone_.minimise;
    close_.onClick = standalone_.close;
    minimise_.setExplicitFocusOrder(20);
    close_.setExplicitFocusOrder(21);
    inspector_.setStandalone(enabled);
    inspector_.audioSettings.onClick = standalone_.audioSettings;
    inspector_.saveState.onClick = standalone_.saveState;
    inspector_.loadState.onClick = standalone_.loadState;
    inspector_.resetState.onClick = standalone_.resetState;
    // The standalone shell supplies an invisible resizer along the cassette edge.
    setResizable(true, !enabled);
    getConstrainer()->setFixedAspectRatio(enabled ? double(view::canvasWidth) / view::canvasHeight
                                                  : 0.0);
    resized();
    repaint();
}
void PluginEditor::setStandaloneInputMuted(bool muted) {
    inspector_.inputMuted.setText(muted ? "INPUT MUTED" : "", juce::dontSendNotification);
    inspector_.inputMuted.setVisible(muted);
    details_.setTooltip(muted ? "settings; audio input is muted — open Audio / MIDI to unmute"
                              : "settings, interface scale and diagnostics");
}
void PluginEditor::showSettings() {
    if (inspector_.isVisible()) {
        inspector_.close.grabKeyboardFocus();
        return;
    }
    inspector_.setVisible(true);
    inspector_.toFront(false);
    inspector_.body.setText(inspectorText(processor_.controls()), false);
    inspector_.enterModalState(false);
    inspector_.close.grabKeyboardFocus();
}
bool PluginEditor::hitTest(int x, int y) {
    if (!standalone_.close)
        return true;
    const auto p = juce::Point<float>(float(x), float(y))
                       .transformedBy(view::canvasTransform(getWidth(), getHeight()).inverted());
    return tape::outline().contains(p);
}
bool PluginEditor::isWindowDragPoint(juce::Point<int> p) {
    auto* c = getComponentAt(p);
    return standalone_.close && (c == this || c == &face_);
}
void PluginEditor::closeInspector() {
    inspector_.exitModalState(0);
    inspector_.setVisible(false);
    details_.grabKeyboardFocus();
}
bool PluginEditor::keyPressed(const juce::KeyPress& k) {
    if (dynamic_cast<juce::TextEditor*>(juce::Component::getCurrentlyFocusedComponent()))
        return false;
    if (k.getModifiers().isCommandDown()) {
        if (k.getTextCharacter() == 'z' || k.getTextCharacter() == 'Z') {
            if (k.getModifiers().isShiftDown())
                processor_.undo.redo();
            else
                processor_.undo.undo();
            return true;
        }
#if JUCE_WINDOWS
        if (k.getTextCharacter() == 'y' || k.getTextCharacter() == 'Y') {
            processor_.undo.redo();
            return true;
        }
#endif
    }
    return false;
}
void PluginEditor::visibilityChanged() {
    syncVisibility();
}
void PluginEditor::parentHierarchyChanged() {
    syncVisibility();
}
void PluginEditor::syncVisibility() {
    const bool active = isShowing();
    if (active == editorActive_)
        return;
    editorActive_ = active;
    if (active) {
        view_ = {};
        startTimerHz(30);
    } else {
        stopTimer();
        for (auto* slider : {&flux_, &bias_, &output_})
            slider->finishGesture();
        if (register_.getToggleState() || snapshot_.audio.engine.brake)
            static_cast<void>(processor_.button(true, false));
        brakePending_ = false;
    }
}
void PluginEditor::timerCallback() {
    if (!isShowing()) {
        stopTimer();
        return;
    }
    updateView(processor_.readTelemetry(), juce::Time::getMillisecondCounterHiRes());
}
void PluginEditor::updateView(const TelemetrySnapshot& snapshot, double now) {
    for (auto* attachment : std::array<ControlAttachment*, 7>{
             fluxAttachment_.get(), biasAttachment_.get(), outputAttachment_.get(),
             driveAttachment_.get(), stateAttachment_.get(), bypassAttachment_.get(),
             domainAttachment_.get()})
        attachment->poll();
    snapshot_ = snapshot;
    view_.update(snapshot, now);
    const auto& next = snapshot.audio.engine;
    if (next.word != register_.display.word || next.domain != register_.display.domain ||
        next.preview != register_.display.preview || view_.live != register_.known ||
        view_.stale != register_.stale) {
        register_.display = next;
        register_.known = view_.live;
        register_.stale = view_.stale;
        register_.repaint();
    }
    const auto controls = processor_.controls();
    const auto& v = controls.values;
    const auto d = view::domain(controls);
    fluxEntry_.target(d, v[0]);
    biasEntry_.target(d, v[1]);
    outputEntry_.target(d, v[5]);
    text(validation_, fluxEntry_.message().isNotEmpty()   ? fluxEntry_.message()
                      : biasEntry_.message().isNotEmpty() ? biasEntry_.message()
                                                          : outputEntry_.message());
    const bool invalid = validation_.getText().isNotEmpty();
    validation_.setVisible(invalid);
    text(context_, !view_.available() ? "NO DATA"
                   : next.brake       ? "BRAKE"
                   : next.running     ? "RUN"
                                      : "PAUSED");
    context_.setTooltip(view::macro(d, v[1]) + "; sampled DSP transport");
    if (brakePending_ && view_.available() && next.brake == register_.getToggleState())
        brakePending_ = false;
    if (!brakePending_ && view_.available())
        register_.setToggleState(next.brake, juce::dontSendNotification);
    register_.setDescription(brakePending_                ? "brake pending"
                             : register_.getToggleState() ? "brake on"
                                                          : "brake off");
    drive_.setDescription(v[2] != 0 ? "running; press to pause" : "paused; press to run");
    state_.setDescription(v[3] != 0 ? "alternate register 1" : "alternate register 0");
    bypass_.setDescription(v[6] != 0 ? "dry input; wet output trim does not apply" : "wet output");
    for (unsigned i = 0; i < 3; ++i)
        domains_[i].setToggleState(i == d, juce::dontSendNotification);
    // Connectivity is unavailable in the immutable snapshot; never infer a disconnected bus from
    // silence.
    text(input_, "NO INPUT");
    input_.setVisible(d == 2 && view_.available() && view_.quietSince >= 0 &&
                      now - view_.quietSince >= 500);
    input_.setTooltip(
        d == 2 ? "route stereo input; quiet is observed silence, not proof of disconnection"
               : "generators can run with no input; connectivity unknown");
    text(limiting_, view_.clipUntil > now ? "LIMITING" : "");
    limiting_.setVisible(view_.clipUntil > now);
    limiting_.setTooltip(v[6] != 0 ? "held 1 second; dry bypass is untrimmed; lower upstream"
                                   : "held 1 second; lower wet OUTPUT");
    text(rejection_, "REJECTED " + juce::String(view_.rejected));
    rejection_.setVisible(view_.unresolved);
    rejection_.setTooltip("targets may differ from sound; make a valid edit or reset; reset clears "
                          "audio; DETAILS acknowledges only the UI marker");
    text(health_, view_.health());
    const auto facts =
        juce::String(view_.unresolved ? "New rejection; targets may differ from sound. " : "") +
        (view_.clipUntil > now ? "Limiting; lower output or upstream dry signal. " : "") +
        (view_.stale ? "Telemetry stale; host may be idle." : "");
    if (facts != announcedFacts_ && now - lastAnnouncement_ >= 1000) {
        if (facts.isNotEmpty() && isShowing())
            juce::AccessibilityHandler::postAnnouncement(
                facts, juce::AccessibilityHandler::AnnouncementPriority::low);
        announcedFacts_ = facts;
        lastAnnouncement_ = now;
    }
    undo_.setEnabled(processor_.undo.canUndo());
    redo_.setEnabled(processor_.undo.canRedo());
    const auto program =
        static_cast<std::size_t>(juce::jlimit(0, 3, processor_.getCurrentProgram()));
    const bool modified = controls != contract::presets[program].state;
    for (std::size_t i = 0; i < contract::presets.size(); ++i) {
        const auto name =
            juce::String(contract::presets[i].name.data()) + (modified && i == program ? " *" : "");
        if (preset_.getItemText(static_cast<int>(i)) != name)
            preset_.changeItemText(static_cast<int>(i + 1), name);
    }
    preset_.setSelectedId(static_cast<int>(program + 1), juce::dontSendNotification);
    preset_.setDescription(modified ? "modified host targets; parameter-only recall clears audio"
                                    : "factory host targets; parameter-only recall clears audio");
    if (inspector_.isVisible()) {
        inspector_.health = view_;
        inspector_.repaint(12, 200, 536, 54);
        const auto content = inspectorText(controls);
        if (inspector_.body.getText() != content) {
            const auto caret = inspector_.body.getCaretPosition();
            inspector_.body.setText(content, false);
            inspector_.body.setCaretPosition(caret);
        }
    }
}
juce::String PluginEditor::inspectorText(const contract::State& s) const {
    const auto d = view::domain(s);
    const auto& v = s.values;
    juce::String result = "SAVED HOST TARGETS\n";
    for (const auto& p : contract::catalogue) {
        const auto f = v[contract::index(p.id)];
        result += juce::String(p.label.data()) + " ";
        if (p.id == contract::Id::Flux || p.id == contract::Id::Bias ||
            p.id == contract::Id::Output)
            result += view::format(p.id, d, f) + " / ";
        result += juce::String(f, 6) + " normalized\n";
    }
    result += "\nBIAS is a coupled macro: " + view::macro(d, v[1]) + ".\n";
    if (d == 1)
        result += "four generations per beat; STATE selects alternate pitch behavior.\n";
    if (d == 2)
        result += "effective target speed " +
                  juce::String(view::fluxValue(d, v[0]) * (1 - 2 * v[3]), 2) +
                  " X; STATE reverses relative sign. Zero FLUX stops writing; brake freezes "
                  "writing while reading can continue.\n";
    result += "\nDSP / RECOVERY\n" + view_.health() + "; connectivity unknown.\n";
    if (view_.available())
        result += "DSP domain " + juce::String(static_cast<int>(snapshot_.audio.engine.domain)) +
                  " / target " + juce::String(d) + "; DSP " +
                  (snapshot_.audio.engine.running ? "RUN" : "PAUSED") + " / target " +
                  (v[2] != 0 ? "RUN" : "PAUSED") + "; brake " +
                  (snapshot_.audio.engine.brake ? "ON" : "OFF") + ".\n";
    result +=
        "REJECTED " + juce::String(view_.rejected) +
        (view_.unresolved ? " unresolved" : " history") +
        ". Make a valid edit or reset; reset clears audio. No applied-target acknowledgement "
        "exists: host targets may differ from sound. ACKNOWLEDGE clears only this UI marker.\n";
    result += "Display drops " + juce::String(view_.drops) +
              " are display loss, not audio failure. Stale/idle callbacks retain last bits. Meters "
              "are sampled block peaks and can miss intervening maxima; no effective continuous "
              "DSP target is available.\n";
    result += "\nMIDI / TIMING\nCC20 FLUX; CC21 BIAS; CC22 run; CC23 domain. Performance only; not "
              "saved or written to host automation. Step-at-point plus 10 ms smoothing. AU "
              "ordinary/scheduled offsets remain unqualified/block-boundary. Free clock.\n";
    result += "\nRECALL\nPresets, RESET and session recall restart phase, clear tape and voices. "
              "Undo restores parameters only and cannot recover audio. Paused does not imply mute. "
              "Wet OUTPUT does not trim dry bypass; for dry limiting lower upstream.\n";
    return result;
}
} // namespace sys01
