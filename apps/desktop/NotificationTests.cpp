#include "PluginEditor.hpp"

#include <cstdio>
#include <thread>
#ifdef SYS01_NATIVE_RT_PROBE
#include "RealtimeProbe.hpp"
#endif

namespace sys01 {
class EditorFixture {
public:
    static bool notification() {
        PluginProcessor processor;
        PluginEditor editor(processor);
        editor.stopTimer();
        auto& flux = *processor.parameters.getParameter("param_flux");
        auto& bias = *processor.parameters.getParameter("param_bias");
        struct Reentry final : juce::AudioProcessorParameter::Listener {
            juce::RangedAudioParameter& flux;
            juce::RangedAudioParameter& bias;
            bool active = false;
            Reentry(juce::RangedAudioParameter& f, juce::RangedAudioParameter& b)
                : flux(f), bias(b) {}
            void parameterValueChanged(int, float) override {
                if (active)
                    return;
                active = true;
                flux.setValueNotifyingHost(.8F);
                bias.setValueNotifyingHost(.7F);
            }
            void parameterGestureChanged(int, bool) override {}
        } reentry(flux, bias);
        flux.addListener(&reentry); // reverse traversal: nested write precedes attachment
#ifdef SYS01_NATIVE_RT_PROBE
        sys01::probe::Counts first, warm;
#endif
        std::thread audio([&] {
#ifdef SYS01_NATIVE_RT_PROBE
            sys01::probe::begin();
#endif
            flux.setValueNotifyingHost(.2F);
#ifdef SYS01_NATIVE_RT_PROBE
            first = sys01::probe::end();
            sys01::probe::begin();
#endif
            flux.sendValueChangedMessageToListeners(.2F); // late obsolete notification
#ifdef SYS01_NATIVE_RT_PROBE
            warm = sys01::probe::end();
#endif
        });
        audio.join();
        flux.removeListener(&reentry);
        juce::MessageManager::getInstance()->runDispatchLoopUntil(50);
        editor.updateView({}, juce::Time::getMillisecondCounterHiRes());
        const auto raw = processor.parameters.getRawParameterValue("param_flux")->load();
        const bool ordered = flux.getValue() == .8F && raw == .8F &&
                             std::abs(editor.flux_.getValue() - .8) < 1e-6 &&
                             std::abs(editor.bias_.getValue() - .7) < 1e-6;
        std::printf("nested authority=%g raw=%g UI=%g biasUI=%g ordering=%s\n", flux.getValue(),
                    raw, editor.flux_.getValue(), editor.bias_.getValue(),
                    ordered ? "PASS" : "FAIL");
        // Fixed-size publication coalesces arbitrarily dense notifications;
        // editor lifetime is synchronized by the retained parameter lock.
        std::atomic<bool> running{true};
        std::atomic<unsigned> writes{0};
        std::thread producer([&] {
            while (running.load(std::memory_order_acquire)) {
                for (const auto& entry : contract::catalogue)
                    processor.parameters.getParameter(entry.key.data())
                        ->setValueNotifyingHost(
                            (writes.load(std::memory_order_relaxed) & 1U) ? 1.F : 0.F);
                writes.fetch_add(1, std::memory_order_release);
            }
        });
        while (writes.load(std::memory_order_acquire) < 16)
            std::this_thread::yield();
        for (int i = 0; i < 64; ++i) {
            PluginEditor temporary(processor);
            temporary.stopTimer();
            temporary.updateView({}, juce::Time::getMillisecondCounterHiRes());
        }
        running.store(false, std::memory_order_release);
        producer.join();
        // Direct writes need no notification to become visible on a UI poll.
        for (const auto& entry : contract::catalogue)
            processor.parameters.getParameter(entry.key.data())->setValue(entry.initial);
        editor.updateView({}, juce::Time::getMillisecondCounterHiRes());
        const bool final =
            std::abs(editor.flux_.getValue() - contract::catalogue[0].initial) < 1e-6 &&
            std::abs(editor.bias_.getValue() - contract::catalogue[1].initial) < 1e-6 &&
            editor.drive_.getToggleState() == (contract::catalogue[2].initial != 0) &&
            editor.state_.getToggleState() == (contract::catalogue[3].initial != 0) &&
            editor.domain_.getSelectedItemIndex() ==
                juce::roundToInt(contract::catalogue[4].initial * 2) &&
            std::abs(editor.output_.getValue() - contract::catalogue[5].initial) < 1e-6 &&
            editor.bypass_.getToggleState() == (contract::catalogue[6].initial != 0);
        std::printf(
            "64 concurrent editor attach/detach; batches=%u; seven-key direct readback=%s\n",
            writes.load(), final ? "PASS" : "FAIL");
#ifdef SYS01_NATIVE_RT_PROBE
        std::printf("attachment first alloc=%llu free=%llu mutex=%llu unfair=%llu; warm alloc=%llu "
                    "free=%llu mutex=%llu unfair=%llu\n",
                    first.allocation, first.deallocation, first.mutex, first.unfair,
                    warm.allocation, warm.deallocation, warm.mutex, warm.unfair);
        if (first.allocation)
            sys01::probe::printAllocation(first);
        return ordered && final && first.allocation == 0 && first.deallocation == 0 &&
               first.unfair == 0 && warm.allocation == 0 && warm.deallocation == 0 &&
               warm.unfair == 0;
#else
        return ordered && final;
#endif
    }
};
} // namespace sys01
int main() {
    juce::ScopedJuceInitialiser_GUI gui;
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
#endif
    return sys01::EditorFixture::notification() ? 0 : 1;
}
