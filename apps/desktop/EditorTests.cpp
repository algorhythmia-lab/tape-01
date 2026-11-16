#include "PluginEditor.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <juce_cryptography/juce_cryptography.h>
namespace sys01 {
namespace {
void require(bool v, const char* message) {
    if (!v) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
struct Listener final : juce::AudioProcessorParameter::Listener {
    int begins = 0, ends = 0, writes = 0;
    void parameterValueChanged(int, float) override {
        ++writes;
    }
    void parameterGestureChanged(int, bool start) override {
        start ? ++begins : ++ends;
    }
};
} // namespace
class EditorFixture {
public:
    static void recallUndo() {
        PluginProcessor p;
        p.prepareToPlay(48000, 4096);
        PluginEditor e(p);
        e.stopTimer();
        p.setCurrentProgram(3);
        juce::AudioBuffer<float> b(2, 4096);
        juce::MidiBuffer midi;
        for (int block = 0; block < 24; ++block) {
            for (int c = 0; c < 2; ++c)
                for (int i = 0; i < 4096; ++i)
                    b.setSample(c, i, .12589254F * std::sin(static_cast<float>(i) * .031F));
            p.processBlock(b, midi);
        }
        b.clear();
        p.processBlock(b, midi);
        require(b.getMagnitude(0, 4096) > .01F, "undo fixture failed to populate tape");
        static_cast<void>(p.parameters.copyState());
        p.undo.clearUndoHistory();
        const auto captured = p.controls();
        e.reset_.onClick();
        require(p.controls() == contract::State{}, "UI reset parameters differ");
        b.clear();
        p.processBlock(b, midi);
        require(b.getMagnitude(0, 4096) == 0, "UI reset did not clear sound");
        e.undo_.onClick();
        require(p.controls() == captured, "UI undo did not restore reset parameters");
        b.clear();
        p.processBlock(b, midi);
        // Undo applies parameter edits, including the accepted 10 ms domain/run
        // ramps. The departing bytebeat domain can sound during that transition;
        // it is not recovered tape. Assert silence after that existing ramp.
        std::printf("UI undo departing-domain peak during 10 ms ramp: %.9g\n",
                    static_cast<double>(b.getMagnitude(0, 512)));
        require(b.getMagnitude(512, 4096 - 512) == 0,
                "UI undo recovered captured tape after domain ramp");
        e.preset_.setSelectedId(2, juce::sendNotificationSync);
        require(p.controls() == contract::presets[1].state, "UI study recall failed");
        b.clear();
        p.processBlock(b, midi);
        require(b.getMagnitude(0, 4096) > 0, "UI generator study remained silent");
        e.undo_.onClick();
        require(p.controls() == captured, "UI undo did not restore study parameters");
        // Undo is a smoothed parameter edit; settle the domain crossfade. It must
        // never resurrect the pre-reset input history.
        for (int block = 0; block < 3; ++block) {
            b.clear();
            p.processBlock(b, midi);
        }
        require(b.getMagnitude(0, 4096) == 0, "study undo recovered destroyed tape");
        std::puts("UI reset/study undo: parameters restored; destroyed tape stays empty PASS");
    }
    static void run(const juce::File& folder, bool controlsOnly) {
        recallUndo();
        folder.createDirectory();
        PluginProcessor p;
        p.prepareToPlay(48000, 64);
        PluginEditor e(p);
        e.stopTimer();
        e.addToDesktop(juce::ComponentPeer::windowIsTemporary);
        e.setVisible(true);
        e.stopTimer();
        require(e.preset_.getX() == e.domains_[1].getX() &&
                    e.preset_.getRight() == e.domains_[2].getRight() &&
                    e.bypass_.getRight() == e.domains_[2].getRight(),
                "preset and bypass edges must align with domain columns");
        require(e.input_.getX() == e.outputLabel_.getX() &&
                    e.input_.getBottom() <= e.outputLabel_.getY(),
                "NO INPUT must sit directly above OUTPUT");
        require(e.face_.getComponentAt(e.register_.getBounds().getCentre()) == &e.register_ &&
                    e.face_.getComponentAt(e.register_.getX() + 60, e.register_.getBottom() - 4) ==
                        &e.register_,
                "entire register window must receive brake clicks");
        auto* brake = e.register_.getAccessibilityHandler();
        require(brake->getRole() == juce::AccessibilityRole::toggleButton,
                "unlabelled register brake must remain an accessible toggle");
        juce::AudioBuffer<float> brakeBuffer(2, 64);
        juce::MidiBuffer brakeMidi;
        const auto processBrake = [&] {
            brakeBuffer.clear();
            p.processBlock(brakeBuffer, brakeMidi);
            return p.readTelemetry();
        };
        const auto savedTargets = p.controls();
        require(brake->getActions().invoke(juce::AccessibilityActionType::toggle),
                "register brake accessibility action missing");
        require(processBrake().audio.engine.brake && brake->getCurrentState().isChecked(),
                "register toggle did not engage brake");
        require(brake->getActions().invoke(juce::AccessibilityActionType::toggle),
                "register brake release action missing");
        require(!processBrake().audio.engine.brake && !brake->getCurrentState().isChecked(),
                "register toggle did not release brake");
        e.register_.setToggleState(true, juce::sendNotificationSync);
        require(processBrake().audio.engine.brake, "register click did not engage brake");
        e.setVisible(false);
        require(!processBrake().audio.engine.brake, "hiding editor left register brake engaged");
        e.setVisible(true);
        e.stopTimer();
        e.updateView(processBrake(), 0);
        e.updateView(processBrake(), 34);
        require(!e.register_.getToggleState() && p.controls() == savedTargets,
                "brake must stay transient and resynchronise after reopening");
        Listener listener;
        auto* parameter = p.parameters.getParameter("param_flux");
        parameter->addListener(&listener);
        e.flux_.commit(.6);
        require(listener.begins == 1 && listener.ends == 1 && listener.writes == 1,
                "numeric gesture unbalanced");
        const auto before = p.controls();
        e.fluxEntry_.focusGained(juce::Component::focusChangedDirectly);
        e.fluxEntry_.setText("999999 TICKS/S");
        require(!e.fluxEntry_.accept() && p.controls() == before,
                "invalid numeric input wrote parameter");
        e.fluxEntry_.cancel();
        require(listener.begins == 1, "cancel sent gesture");
        e.fluxEntry_.focusGained(juce::Component::focusChangedDirectly);
        e.fluxEntry_.setText("5000 TICKS/S");
        parameter->setValueNotifyingHost(.2F);
        e.updateView({}, 100);
        require(e.fluxEntry_.getText() == "5000 TICKS/S", "automation replaced edit buffer");
        require(e.fluxEntry_.accept() && std::abs(p.controls().values[0] - .6F) < 1e-6F,
                "numeric entry not committed");
        e.fluxEntry_.focusGained(juce::Component::focusChangedDirectly);
        e.fluxEntry_.setText("5500 TICKS/S");
        p.parameters.getParameter("engine_state")->setValueNotifyingHost(.5F);
        e.updateView({}, 133);
        require(e.fluxEntry_.getText().contains("BPM"), "domain change reinterpreted old unit");
        e.flux_.keyPressed(juce::KeyPress(juce::KeyPress::upKey));
        e.flux_.keyPressed(juce::KeyPress(juce::KeyPress::upKey));
        e.flux_.finishGesture();
        require(listener.begins == listener.ends, "held key gesture unbalanced");
        const auto afterKeys = p.controls().values[0];
        require(p.undo.undo(), "undo unavailable");
        require(std::abs(p.controls().values[0] - (afterKeys - .02F)) < 1e-5F,
                "key repeats not one undo group");
        e.flux_.commit(.7);
        require(p.undo.undo() && p.undo.redo(), "parameter undo/redo failed");
        const auto beforeAT = listener.begins;
        e.flux_.getAccessibilityHandler()->getValueInterface()->setValueAsString("NaN");
        require(listener.begins == beforeAT, "invalid accessibility input sent gesture");
        e.flux_.getAccessibilityHandler()->getValueInterface()->setValueAsString("165 BPM");
        require(listener.begins == beforeAT + 1 && listener.begins == listener.ends,
                "accessible slider edit not balanced");
        const auto beforeTimer = listener.begins;
        e.updateView({}, 166);
        require(listener.begins == beforeTimer, "timer sent gesture");
        // Close with held keyboard gesture; attachment must still receive the final end.
        {
            PluginEditor closing(p);
            closing.stopTimer();
            auto* s = &closing.flux_;
            s->keyPressed(juce::KeyPress(juce::KeyPress::downKey));
            closing.register_.setToggleState(true, juce::sendNotificationSync);
            require(processBrake().audio.engine.brake, "close fixture failed to engage brake");
        }
        require(!processBrake().audio.engine.brake, "closing editor left register brake engaged");
        require(listener.begins == listener.ends, "close mid-gesture not balanced");
        parameter->removeListener(&listener);
        e.fluxEntry_.cancel();
        require(theme::mono(14).getTypefacePtr()->getName() == "JetBrains Mono" &&
                    theme::display(22).getTypefacePtr()->getName() == "Barlow Condensed" &&
                    theme::body(14).getTypefacePtr()->getName() == "Inter",
                "webpage typefaces did not load from embedded resources");
        require(e.reset_.getParentComponent() == &e.inspector_ &&
                    e.scale_.getParentComponent() == &e.inspector_ && !e.inspector_.isVisible(),
                "secondary controls must be available inside settings");
        e.details_.onClick();
        require(e.inspector_.isVisible() && e.reset_.isShowing() && e.scale_.isShowing(),
                "settings did not reveal secondary controls");
        e.closeInspector();
        require(!e.inspector_.isVisible(), "settings did not close");
        const auto f = theme::mono(14).getTypefacePtr();
        juce::String report = "native editor fixture; embedded typeface: " + f->getName() + " / " +
                              f->getStyle() + "\n";
        report +=
            "register brake toggle, accessible action, full-window hit area, hide/close release, "
            "aligned columns, gesture, numeric validation, automation typing, domain "
            "cancellation, key "
            "repeat, undo/redo, close and timer tests PASS\n";
        if (controlsOnly) {
            e.setVisible(false);
            require(!e.isTimerRunning(), "hidden editor timer still running");
            e.setVisible(true);
            require(e.isTimerRunning(), "reopened editor did not restart bounded telemetry");
            e.stopTimer();
            e.removeFromDesktop();
            report +=
                "controls-only fixture: full image/scale matrix omitted; no paint budget claim.\n";
            folder.getChildFile("native-editor-controls-report.txt").replaceWithText(report);
            std::printf("%s", report.toRawUTF8());
            return;
        }
        const std::array<float, 4> scales{1, 1.25F, 1.5F, 2};
        std::ofstream hashes(
            folder.getChildFile("render-hashes.csv").getFullPathName().toStdString());
        hashes << "case,domain,run,state,brake,bypass,input_missing,clip,modified,rejected,scale,"
                  "sha256\n";
        std::ofstream extents(
            folder.getChildFile("glyph-extents.csv").getFullPathName().toStdString());
        extents << "text,width,available\n";
        const auto measure = [&](const juce::String& value, const juce::Font& font, int available) {
            juce::GlyphArrangement glyphs;
            glyphs.addLineOfText(font, value, 0, 0);
            const auto width = glyphs.getBoundingBox(0, -1, true).getWidth();
            extents << value << ',' << width << ',' << available << '\n';
            require(width <= available, "native glyph extent exceeds essential label bounds");
        };
        double maxPaint = 0, maxTick = 0, totalPaint = 0;
        int count = 0;
        double now = 1000;
        for (unsigned d = 0; d < 3; ++d)
            for (unsigned flags = 0; flags < 256; ++flags)
                for (float scale : scales) {
                    auto s = contract::presets[0].state;
                    s.values[4] = d * .5F;
                    s.values[2] = (flags & 1) ? 1 : 0;
                    s.values[3] = (flags & 2) ? 1 : 0;
                    s.values[6] = (flags & 8) ? 1 : 0;
                    s.values[0] = (flags & 64) ? .27F : .5F;
                    for (const auto& param : contract::catalogue)
                        p.parameters.getParameter(param.key.data())
                            ->setValueNotifyingHost(s.values[contract::index(param.id)]);
                    TelemetrySnapshot snapshot;
                    snapshot.block = 100 + static_cast<std::uint64_t>(count) * 2;
                    snapshot.frames = 64;
                    snapshot.audio.engine.domain = static_cast<algorhythmia::SceneId>(d);
                    snapshot.audio.engine.word = d == 0 ? 0x81 : d == 1 ? 110 : 0x18;
                    snapshot.audio.engine.running = flags & 1;
                    snapshot.audio.engine.brake = flags & 4;
                    snapshot.audio.clipped = flags & 32;
                    snapshot.audio.inputPeak = (flags & 16) ? 0 : .5F;
                    snapshot.audio.inputChannelPeak = {(flags & 16) ? 0.F : .25F,
                                                       (flags & 16) ? 0.F : .5F};
                    snapshot.audio.outputChannelPeak = {.4F, .8F};
                    snapshot.rejections = (flags & 128) ? 4294967295U : 0;
                    e.view_ = {};
                    e.brakePending_ = false;
                    e.updateView(snapshot, now);
                    ++snapshot.block;
                    const auto tickStart = juce::Time::getMillisecondCounterHiRes();
                    e.updateView(snapshot, now + 34);
                    maxTick =
                        std::max(maxTick, juce::Time::getMillisecondCounterHiRes() - tickStart);
                    e.setSize(juce::roundToInt(view::canvasWidth * scale),
                              juce::roundToInt(view::canvasHeight * scale));
                    require(e.face_.getTransform().mat00 == e.face_.getTransform().mat11,
                            "nonuniform transform");
                    require(e.scale_.getText() == juce::String(scale * 100, 0) + "%",
                            "scale indicator disagrees");
                    if (scale == 1) {
                        for (auto* label :
                             {&e.input_, &e.limiting_, &e.rejection_, &e.health_, &e.recall_})
                            if (label->isVisible())
                                measure(label->getText(), label->getFont(), label->getWidth());
                        for (auto* entry : {&e.fluxEntry_, &e.biasEntry_, &e.outputEntry_})
                            measure(entry->getText(), entry->getFont(), entry->getWidth() - 8);
                        for (auto* button : {&e.drive_, &e.state_, &e.bypass_})
                            measure(button->getButtonText(), theme::mono(12),
                                    button->getWidth() - 8);
                    }
                    const auto paintStart = juce::Time::getMillisecondCounterHiRes();
                    const auto image = e.createComponentSnapshot(e.getLocalBounds());
                    const auto paintMs = juce::Time::getMillisecondCounterHiRes() - paintStart;
                    maxPaint = std::max(maxPaint, paintMs);
                    totalPaint += paintMs;
                    juce::Image::BitmapData data(image, juce::Image::BitmapData::readOnly);
                    const auto hash =
                        juce::SHA256(data.data, static_cast<std::size_t>(data.size)).toHexString();
                    hashes << count << ',' << d;
                    for (unsigned i = 0; i < 8; ++i)
                        hashes << ',' << ((flags >> i) & 1U);
                    hashes << ',' << scale << ',' << hash << '\n';
                    if (flags == 0 || flags == 255)
                        save(folder, e,
                             "domain-" + juce::String(d) + (flags ? "-all" : "-idle") + "-" +
                                 juce::String(scale * 100, 0));
                    require(e.rejection_.getText().contains(juce::String(snapshot.rejections)),
                            "rejection count hidden");
                    require(!(flags & 32) || e.limiting_.getText() == "LIMITING",
                            "clipping masked by other status");
                    ++count;
                    now += 100;
                }
        require(count == 3072, "scenario matrix incomplete");
        report += "3072 native renders plus all 256 register words and embedded font extents; "
                  "quiet transform input is an observation, not inferred bus connectivity. "
                  "Secondary controls verified inside settings. All four scales.\n";
        for (float scale : scales) {
            e.setSize(juce::roundToInt(view::canvasWidth * scale),
                      juce::roundToInt(view::canvasHeight * scale));
            e.flux_.grabKeyboardFocus();
            save(folder, e, "flux-focus-" + juce::String(scale * 100, 0));
            e.bias_.grabKeyboardFocus();
            save(folder, e, "bias-focus-" + juce::String(scale * 100, 0));
            e.fluxEntry_.focusGained(juce::Component::focusChangedDirectly);
            e.fluxEntry_.setText("NaN");
            require(!e.fluxEntry_.accept(), "invalid fixture accepted");
            e.updateView(e.snapshot_, now);
            save(folder, e, "invalid-entry-" + juce::String(scale * 100, 0));
            e.fluxEntry_.cancel();
            e.view_ = {};
            e.updateView({}, now);
            save(folder, e, "unknown-" + juce::String(scale * 100, 0));
            TelemetrySnapshot s;
            s.block = 10;
            s.audio.engine.word = 0xc0;
            s.audio.engine.preview = true;
            e.updateView(s, now + 34);
            ++s.block;
            e.updateView(s, now + 68);
            save(folder, e, "preview-" + juce::String(scale * 100, 0));
            e.updateView(s, now + 400);
            save(folder, e, "stale-" + juce::String(scale * 100, 0));
            s.block++;
            s.rejections = 1;
            s.audio.clipped = true;
            s.audio.engine.brake = true;
            e.updateView(s, now + 434);
            save(folder, e, "rejection-clip-brake-" + juce::String(scale * 100, 0));
            e.inspector_.setVisible(true);
            e.inspector_.body.setText(e.inspectorText(p.controls()));
            e.inspector_.health = e.view_;
            save(folder, e, "details-" + juce::String(scale * 100, 0));
            e.inspector_.setVisible(false);
            e.view_ = {};
            s.audio.engine.domain = algorhythmia::SceneId::LiveTransform;
            s.audio.engine.running = true;
            s.audio.engine.brake = false;
            s.audio.clipped = false;
            s.rejections = 0;
            ++s.block;
            e.updateView(s, now + 1000);
            ++s.block;
            e.updateView(s, now + 1034);
            ++s.block;
            e.updateView(s, now + 1600);
            require(e.input_.isVisible() && e.input_.getText() == "NO INPUT",
                    "quiet transform input must show NO INPUT in the output bay");
            save(folder, e, "no-input-" + juce::String(scale * 100, 0));
        }
        for (unsigned word = 0; word < 256; ++word) {
            e.register_.display.word = static_cast<std::uint8_t>(word);
            e.register_.known = true;
            e.register_.stale = false;
            const auto description = e.register_.getAccessibilityHandler()->getDescription();
            require(
                description.contains("hex " + juce::String::toHexString(static_cast<int>(word))),
                "accessible register value incorrect");
            const auto image = e.register_.createComponentSnapshot(e.register_.getLocalBounds());
            juce::Image::BitmapData pixels(image, juce::Image::BitmapData::readOnly);
            hashes << "word-" << word << ",,,,,,,,,,,"
                   << juce::SHA256(pixels.data, static_cast<std::size_t>(pixels.size)).toHexString()
                   << '\n';
        }
        e.setSize(1000, 700);
        require(e.face_.getTransform().mat00 == 1.5625F && e.face_.getTransform().mat12 == 25.F,
                "letterbox incorrect");
        save(folder, e, "letterbox-1000x700");
        report += "unpaced offscreen one-instance CPU rendering; max updateView ms " +
                  juce::String(maxTick, 4) + "; max full snapshot paint ms " +
                  juce::String(maxPaint, 4) + "; mean " + juce::String(totalPaint / count, 4) +
                  ". Not DAW/driver performance qualification.\n";
        report += "Actual native screen reader, DAW keyboard/gesture recordings, host resize "
                  "refusal, Windows and musician tests are not performed by this fixture.\n";
        folder.getChildFile("native-editor-report.txt").replaceWithText(report);
        std::printf("%s", report.toRawUTF8());
        e.setVisible(false);
        require(!e.isTimerRunning(), "hidden editor timer still running");
        e.setVisible(true);
        require(e.isTimerRunning(), "reopened editor did not restart bounded telemetry");
        e.stopTimer();
        e.removeFromDesktop();
    }

private:
    static void save(const juce::File& folder, PluginEditor& e, const juce::String& name) {
        const auto image = e.createComponentSnapshot(e.getLocalBounds());
        auto stream = folder.getChildFile(name + ".png").createOutputStream();
        require(stream != nullptr, "snapshot open failed");
        stream->truncate();
        require(juce::PNGImageFormat{}.writeImageToStream(image, *stream), "snapshot write failed");
    }
};
} // namespace sys01
int main(int argc, char** argv) {
    juce::ScopedJuceInitialiser_GUI gui;
    using namespace sys01;
    for (unsigned d = 0; d < 3; ++d) {
        for (auto id : {contract::Id::Flux, contract::Id::Bias, contract::Id::Output}) {
            for (float f : {0.F, .25F, .5F, .75F, 1.F}) {
                const auto parsed = view::parse(id, d, view::format(id, d, f));
                require(parsed.value && std::abs(*parsed.value - f) < 1e-5F,
                        "mapped endpoint roundtrip failed");
            }
            for (auto invalid : {"", "NaN", "inf", "1e2", "1,2", "1 + 2", "1wrong", "١",
                                 "999999999999999999999999999999999999999999999999999999999999"})
                require(!view::parse(id, d, juce::String::fromUTF8(invalid)).value,
                        "strict entry accepted invalid text");
        }
    }
    require(view::parse(contract::Id::Flux, 2, "0 X").value == .5F, "zero inverse not canonical");
    require(!view::parse(contract::Id::Flux, 2, "0.01 X").value, "inside detent accepted");
    for (float f : {.495F, .505F, std::nextafter(.495F, 0.F), std::nextafter(.505F, 1.F), .496F})
        require(view::fluxValue(2, f) == algorhythmia::Transform::speedForFlux(f),
                "detent differs from DSP helper");
    for (float f : {0.F, .25F, .5F, .75F, 1.F})
        require(view::macro(1, f).contains(juce::String(algorhythmia::Cellular::ruleForBias(f))),
                "rule helper differs");
    view::Health h;
    TelemetrySnapshot s;
    s.block = 10;
    s.rejections = 0xffffffffU;
    h.update(s, 100);
    require(!h.live && !h.unresolved, "initial history announced as fresh");
    s.block = 11;
    s.rejections = 0;
    s.audio.clipped = true;
    h.update(s, 134);
    require(h.available() && h.unresolved && h.clipUntil == 1134, "wrap/new clip not handled");
    h.update(s, 400);
    require(h.stale, "stale data treated as live");
    s.block = 1;
    h.update(s, 434);
    require(!h.live && !h.unresolved, "reset not rebaselined");
    s.block = 2;
    s.audio.inputChannelPeak = {.25F, .75F};
    s.audio.outputChannelPeak = {.1F, .9F};
    h.update(s, 468);
    require(h.meterDb[0] != h.meterDb[1] && h.meterDb[2] != h.meterDb[3],
            "meter channels duplicated");
    const auto peak = h.meterDb[1];
    s.block = 3;
    s.audio.inputChannelPeak = {0, 0};
    h.update(s, 568);
    require(std::abs(h.meterDb[1] - (peak - 2)) < 1e-4F, "meter fall not elapsed-time based");
    s.block = 4;
    s.droppedSnapshots = 1;
    h.update(s, 602);
    require(!h.meterValid, "lost interval showed meter value");
    require(argc == 2 || (argc == 3 && std::strcmp(argv[2], "--controls-only") == 0),
            "output folder [--controls-only] required");
    EditorFixture::run(juce::File(argv[1]), argc == 3);
}
