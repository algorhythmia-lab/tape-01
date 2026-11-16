#pragma once
#include <juce_graphics/juce_graphics.h>

// Cassette geometry shared with the tape 01 webpage illustration.
// Source profile SHA-256: 7e7775fc243be3c50c181549f7b239df2aac1d46e4328acbfce67cfafeafd19a
// Proportional visual reference; no hardware or dimensional validation is implied.
namespace sys01::tape {
inline constexpr float scale = 6.F, left = 18.8F, top = 16.F;
inline juce::Point<float> point(float x, float y) {
    return {left + x * scale, top + y * scale};
}
inline const juce::Path& outline() {
    static const auto path = [] {
        juce::Path p;
        p.startNewSubPath(point(61.45229F, 63.80000F));
        p.lineTo(point(3.06811F, 63.79189F));
        p.lineTo(point(2.83762F, 63.76402F));
        p.lineTo(point(2.38516F, 63.65508F));
        p.lineTo(point(1.95637F, 63.47722F));
        p.lineTo(point(1.56029F, 63.23451F));
        p.lineTo(point(1.20621F, 62.93301F));
        p.lineTo(point(1.04841F, 62.76275F));
        p.lineTo(point(0.77591F, 62.38576F));
        p.lineTo(point(0.66200F, 62.18257F));
        p.lineTo(point(0.56441F, 61.97279F));
        p.lineTo(point(0.42062F, 61.53093F));
        p.lineTo(point(0.37514F, 61.30139F));
        p.lineTo(point(0.34986F, 61.07083F));
        p.lineTo(point(0.34013F, 57.87514F));
        p.lineTo(point(-0.47122F, 56.71662F));
        p.lineTo(point(-0.47122F, 41.90685F));
        p.lineTo(point(0.34013F, 40.74838F));
        p.lineTo(point(0.34986F, 2.73224F));
        p.lineTo(point(0.37514F, 2.50018F));
        p.lineTo(point(0.42062F, 2.27123F));
        p.lineTo(point(0.56441F, 1.82927F));
        p.lineTo(point(0.66200F, 1.61781F));
        p.lineTo(point(0.90454F, 1.22288F));
        p.lineTo(point(1.04841F, 1.03863F));
        p.lineTo(point(1.20621F, 0.86954F));
        p.lineTo(point(1.56029F, 0.56712F));
        p.lineTo(point(1.75311F, 0.43730F));
        p.lineTo(point(2.16788F, 0.22701F));
        p.lineTo(point(2.61096F, 0.08319F));
        p.lineTo(point(2.83762F, 0.03848F));
        p.lineTo(point(3.30136F, 0.00000F));
        p.lineTo(point(97.09936F, 0.00156F));
        p.lineTo(point(97.56296F, 0.03848F));
        p.lineTo(point(97.78998F, 0.08396F));
        p.lineTo(point(98.23282F, 0.22856F));
        p.lineTo(point(98.64737F, 0.43730F));
        p.lineTo(point(98.84008F, 0.56790F));
        p.lineTo(point(99.19394F, 0.86954F));
        p.lineTo(point(99.49609F, 1.22366F));
        p.lineTo(point(99.73911F, 1.61897F));
        p.lineTo(point(99.91653F, 2.04850F));
        p.lineTo(point(100.02564F, 2.50174F));
        p.lineTo(point(100.05090F, 2.73224F));
        p.lineTo(point(100.06057F, 40.74955F));
        p.lineTo(point(100.87122F, 41.90790F));
        p.lineTo(point(100.87122F, 56.71763F));
        p.lineTo(point(100.06057F, 57.87497F));
        p.lineTo(point(100.06057F, 60.83774F));
        p.lineTo(point(100.02460F, 61.30240F));
        p.lineTo(point(99.91514F, 61.75388F));
        p.lineTo(point(99.83459F, 61.97380F));
        p.lineTo(point(99.62327F, 62.38677F));
        p.lineTo(point(99.35066F, 62.76377F));
        p.lineTo(point(99.19359F, 62.93301F));
        p.lineTo(point(98.83938F, 63.23603F));
        p.lineTo(point(98.44317F, 63.47824F));
        p.lineTo(point(98.01445F, 63.65609F));
        p.lineTo(point(97.78881F, 63.71994F));
        p.lineTo(point(97.33089F, 63.79189F));
        p.lineTo(point(97.09784F, 63.80000F));
        p.closeSubPath();
        return p;
    }();
    return path;
}
inline const juce::Path& head() {
    static const auto path = [] {
        juce::Path p;
        p.startNewSubPath(point(15.54606F, 63.79899F));
        p.lineTo(point(19.78766F, 48.58944F));
        p.lineTo(point(80.61180F, 48.58944F));
        p.lineTo(point(84.85303F, 63.80000F));
        p.closeSubPath();
        return p;
    }();
    return path;
}
} // namespace sys01::tape
