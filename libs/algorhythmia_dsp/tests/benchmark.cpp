#include <algorhythmia/engine.hpp>
#include <array>
#include <chrono>
#include <cstdio>

int main() {
    constexpr std::size_t blockSize = 128;
    constexpr std::size_t blocks = 48000;
    constexpr float rate = 48000.0F;
    std::array<float, blockSize> inputL{}, inputR{}, left{}, right{};
    for (std::size_t i = 0; i < blockSize; ++i) {
        inputL[i] = (static_cast<float>(i) / 64.0F - 1.0F) * 0.4F;
        inputR[i] = -inputL[i];
    }
    algorhythmia::Engine engine;
    constexpr std::array names{"bytebeat", "cellular", "transform"};
    for (unsigned domain = 0; domain < 3; ++domain) {
        engine.init(rate);
        engine.setScene(static_cast<algorhythmia::SceneId>(domain));
        engine.setParameter(algorhythmia::ParameterId::Drive, 1.0F);
        engine.setParameter(algorhythmia::ParameterId::Flux, 0.91F);
        engine.setParameter(algorhythmia::ParameterId::Bias, 1.0F);
        for (unsigned block = 0; block < 512; ++block) {
            engine.process(inputL.data(), inputR.data(), left.data(), right.data(), blockSize);
        }
        double checksum = 0.0;
        const auto start = std::chrono::steady_clock::now();
        for (std::size_t block = 0; block < blocks; ++block) {
            engine.process(inputL.data(), inputR.data(), left.data(), right.data(), blockSize);
            checksum += left[block % blockSize] + right[block % blockSize];
        }
        const double elapsed =
            std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        constexpr auto frames = blocks * blockSize;
        std::printf("%s: %zu stereo frames, %.3f s, %.1f ns/frame, %.1fx realtime, checksum %.9f\n",
                    names[domain], frames, elapsed, elapsed * 1.0e9 / static_cast<double>(frames),
                    static_cast<double>(frames) / rate / elapsed, checksum);
    }
    std::puts(
        "host throughput is informational; hardware callback deadlines need device measurements.");
}
