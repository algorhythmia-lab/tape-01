#include <algorhythmia/engine.hpp>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

int main(int argc, char** argv) {
    double hours = 8.0;
    if (argc == 2) {
        char* end = nullptr;
        hours = std::strtod(argv[1], &end);
        if (end == argv[1] || *end != '\0' || !std::isfinite(hours) || hours <= 0 || hours > 24) {
            return 2;
        }
    } else if (argc != 1) {
        return 2;
    }
    constexpr std::size_t blockSize = 256;
    constexpr float rate = 48000.0F;
    const auto frames = static_cast<std::uint64_t>(hours * 3600.0 * rate);
    std::array<float, blockSize> inputL{}, inputR{}, outputL{}, outputR{};
    algorhythmia::Engine engine;
    constexpr std::array names{"bytebeat", "cellular", "transform"};
    for (unsigned domain = 0; domain < 3; ++domain) {
        engine.init(rate);
        engine.setScene(static_cast<algorhythmia::SceneId>(domain));
        engine.setParameter(algorhythmia::ParameterId::Drive, 1);
        engine.setParameter(algorhythmia::ParameterId::Flux, 1);
        engine.setParameter(algorhythmia::ParameterId::Bias, 1);
        std::uint32_t random = 0x12345678U;
        float peak = 0;
        double energy = 0;
        const auto start = std::chrono::steady_clock::now();
        for (std::uint64_t offset = 0; offset < frames; offset += blockSize) {
            const auto count =
                static_cast<std::size_t>(std::min<std::uint64_t>(blockSize, frames - offset));
            const auto block = offset / blockSize;
            if (block % 2048U == 0) {
                const auto stage = (block / 2048U) % 12U;
                // includes full recursion, both directions, zero-hz freeze, bank
                // steps, sustained brakes, and exact run/pause endpoints.
                constexpr std::array fluxes{1.0F, 0.0F, 0.5F, 0.75F, 0.25F, 0.91F};
                engine.setParameter(algorhythmia::ParameterId::Flux, fluxes[stage % fluxes.size()]);
                engine.setParameter(algorhythmia::ParameterId::Bias,
                                    stage < 8 ? 1.0F : static_cast<float>(stage - 8) / 3.0F);
                engine.setParameter(algorhythmia::ParameterId::State,
                                    static_cast<float>(stage & 1U));
                engine.setParameter(algorhythmia::ParameterId::Drive, stage == 11 ? 0.0F : 1.0F);
                engine.setButton(algorhythmia::ParameterId::Drive, stage == 6);
                engine.setButton(algorhythmia::ParameterId::State, stage == 7);
            }
            for (std::size_t i = 0; i < count; ++i) {
                random = random * 1664525U + 1013904223U;
                inputL[i] = (static_cast<float>(random & 65535U) / 32768.0F - 1.0F) * 0.3F;
                inputR[i] = -inputL[i] * 0.6F;
            }
            if (block % 257U == 0) {
                inputL[0] = std::numeric_limits<float>::quiet_NaN();
            }
            if (block % 263U == 0) {
                inputR[0] = std::numeric_limits<float>::denorm_min();
            }
            engine.process(inputL.data(), inputR.data(), outputL.data(), outputR.data(), count);
            for (std::size_t i = 0; i < count; ++i) {
                for (const auto value : {outputL[i], outputR[i]}) {
                    if (!std::isfinite(value) || std::fpclassify(value) == FP_SUBNORMAL ||
                        std::abs(value) > 1.0F) {
                        std::fprintf(stderr, "%s invalid output at frame %llu\n", names[domain],
                                     static_cast<unsigned long long>(offset + i));
                        return 1;
                    }
                    peak = std::max(peak, std::abs(value));
                    energy += static_cast<double>(value) * value;
                }
            }
        }
        const double elapsed =
            std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (energy == 0) {
            return 1;
        }
        std::printf("%s: %.4f hours, %llu stereo frames, %.3f s wall time, peak %.6f, rms %.6f, "
                    "finite/normal/bounded\n",
                    names[domain], hours, static_cast<unsigned long long>(frames), elapsed, peak,
                    std::sqrt(energy / (2.0 * static_cast<double>(frames))));
        std::fflush(stdout);
    }
}
