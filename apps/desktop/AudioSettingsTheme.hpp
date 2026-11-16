#pragma once
#include "Theme.hpp"
#include "Typography.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace sys01 {
// Presentation only: the original JUCE dialog, controls and device callbacks remain intact.
class AudioSettingsTheme final : private juce::ComponentListener {
public:
    explicit AudioSettingsTheme(juce::DialogWindow& dialog)
        : previous_(&juce::LookAndFeel::getDefaultLookAndFeel()), dialog_(&dialog) {
        dialog.addComponentListener(this);
        dialog.setLookAndFeel(&look_);
        dialog.setBackgroundColour(juce::Colour(theme::basalt));
        // JUCE's channel/MIDI rows create default Fonts directly. Scope their fallback
        // typeface to this modal dialog; the cassette has its own explicit look/fonts.
        juce::LookAndFeel::setDefaultLookAndFeel(&look_);
        juce::Typeface::clearTypefaceCache();
    }
    ~AudioSettingsTheme() override {
        if (dialog_ != nullptr) {
            dialog_->removeComponentListener(this);
            dialog_->setLookAndFeel(nullptr);
        }
        restoreDefault();
    }

private:
    class Look final : public juce::LookAndFeel_V4 {
    public:
        Look()
            : LookAndFeel_V4(ColourScheme{theme::basalt, theme::obsidian, theme::basalt,
                                          theme::slate, theme::bone, theme::bone, theme::pure,
                                          theme::raised, theme::bone}) {
            setDefaultSansSerifTypeface(theme::body(14).getTypefacePtr());
            setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour(theme::pure));
            setColour(juce::ScrollBar::thumbColourId, juce::Colour(theme::muted));
            setColour(juce::ToggleButton::tickDisabledColourId, juce::Colour(theme::muted));
        }
        juce::Font getLabelFont(juce::Label& label) override {
            if (auto* box = dynamic_cast<juce::ComboBox*>(label.getParentComponent()))
                return getComboBoxFont(*box);
            return theme::mono(13);
        }
        juce::Font getComboBoxFont(juce::ComboBox& box) override {
            return theme::mono(juce::jmin(14.F, float(box.getHeight()) * .6F));
        }
        juce::Font getTextButtonFont(juce::TextButton&, int height) override {
            return theme::mono(juce::jmin(14.F, float(height) * .6F));
        }
        juce::Font getPopupMenuFont() override {
            return theme::mono(14);
        }
        juce::Font getAlertWindowTitleFont() override {
            return theme::display(22);
        }
        juce::Font getAlertWindowMessageFont() override {
            return theme::body(14);
        }
        void drawLevelMeter(juce::Graphics& g, int width, int height, float level) override {
            // Preserve the stock meter's seven segments, rounding and full-scale warning.
            g.setColour(juce::Colour(theme::basalt));
            g.fillRoundedRectangle(0, 0, float(width), float(height), 3);
            const int lit = juce::roundToInt(7.F * level);
            const float segment = (float(width) - 4.F) / 7.F;
            for (int i = 0; i < 7; ++i) {
                g.setColour(juce::Colour(i >= lit ? theme::border
                                         : i == 6 ? theme::red
                                                  : theme::bone));
                g.fillRoundedRectangle(2.F + float(i) * segment + .03F * segment, 2, .94F * segment,
                                       float(height) - 4.F, .1F * segment);
            }
        }
    };
    Look look_;
    juce::WeakReference<juce::LookAndFeel> previous_;
    juce::Component::SafePointer<juce::DialogWindow> dialog_;
    void restoreDefault() {
        if (&juce::LookAndFeel::getDefaultLookAndFeel() == &look_) {
            juce::LookAndFeel::setDefaultLookAndFeel(previous_.get());
            juce::Typeface::clearTypefaceCache();
        }
    }
    void componentVisibilityChanged(juce::Component& component) override {
        if (!component.isVisible())
            restoreDefault();
    }
    void componentBeingDeleted(juce::Component&) override {
        dialog_ = nullptr;
        restoreDefault();
    }
};
} // namespace sys01
