#include <algorhythmia/engine.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string_view>

namespace {

constexpr std::uint32_t kSampleRate = 48'000;
constexpr std::uint32_t kDurationSeconds = 5;
constexpr std::uint16_t kChannels = 2;
constexpr std::uint16_t kBitsPerSample = 16;
constexpr std::uint16_t kBytesPerFrame = kChannels * (kBitsPerSample / 8);
constexpr std::uint32_t kFrames = kSampleRate * kDurationSeconds;
constexpr std::uint32_t kDataBytes = kFrames * kBytesPerFrame;
constexpr std::size_t kBlockFrames = 64;
static_assert(kDataBytes <= std::numeric_limits<std::uint32_t>::max() - 36U);

void writeU16(std::ostream& stream, std::uint16_t value) {
    const std::array<char, 2> bytes{static_cast<char>(value & 0xffU),
                                    static_cast<char>((value >> 8U) & 0xffU)};
    stream.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

void writeU32(std::ostream& stream, std::uint32_t value) {
    writeU16(stream, static_cast<std::uint16_t>(value & 0xffffU));
    writeU16(stream, static_cast<std::uint16_t>((value >> 16U) & 0xffffU));
}

void writeHeader(std::ostream& stream) {
    // Serialize the PCM RIFF header field by field; do not write a native struct.
    stream.write("RIFF", 4);
    writeU32(stream, 36U + kDataBytes);
    stream.write("WAVEfmt ", 8);
    writeU32(stream, 16);
    writeU16(stream, 1); // Linear PCM.
    writeU16(stream, kChannels);
    writeU32(stream, kSampleRate);
    writeU32(stream, kSampleRate * kBytesPerFrame);
    writeU16(stream, kBytesPerFrame);
    writeU16(stream, kBitsPerSample);
    stream.write("data", 4);
    writeU32(stream, kDataBytes);
}

std::int16_t quantize(float sample) {
    if (!std::isfinite(sample)) {
        throw std::runtime_error("The DSP produced a non-finite sample");
    }
    const float bounded = std::clamp(sample, -1.0F, 1.0F);
    if (bounded <= -1.0F) {
        return std::numeric_limits<std::int16_t>::min();
    }
    return static_cast<std::int16_t>(std::lround(bounded * 32767.0F));
}

void render(const char* path) {
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        throw std::runtime_error("Cannot open the output WAV file");
    }
    output.exceptions(std::ios::badbit | std::ios::failbit);
    writeHeader(output);

    algorhythmia::Engine engine;
    engine.init(static_cast<float>(kSampleRate));
    engine.setScene(algorhythmia::SceneId::Bytebeat);
    engine.setParameter(algorhythmia::ParameterId::Flux, 0.5F);
    engine.setParameter(algorhythmia::ParameterId::Bias, 0.35F);
    engine.setParameter(algorhythmia::ParameterId::Drive, 1.0F);

    std::array<float, kBlockFrames> left{};
    std::array<float, kBlockFrames> right{};
    for (std::size_t rendered = 0; rendered < kFrames; rendered += kBlockFrames) {
        const auto count = std::min(kBlockFrames, static_cast<std::size_t>(kFrames) - rendered);
        engine.process(nullptr, nullptr, left.data(), right.data(), count);
        for (std::size_t frame = 0; frame < count; ++frame) {
            // Conversion to unsigned preserves the signed PCM bit pattern modulo 2^16.
            writeU16(output, static_cast<std::uint16_t>(quantize(left[frame])));
            writeU16(output, static_cast<std::uint16_t>(quantize(right[frame])));
        }
    }
    output.flush();
    output.close();
}

} // namespace

int main(int argc, char** argv) {
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        std::cout << "Usage: algorhythmia_render [output.wav]\n"
                     "Render 5 seconds of 48 kHz stereo PCM16 bytebeat.\n";
        return 0;
    }
    if (argc > 2) {
        std::cerr << "Usage: algorhythmia_render [output.wav]\n";
        return 2;
    }
    const char* path = argc == 2 ? argv[1] : "tape01-bytebeat.wav";
    try {
        render(path);
        std::cout << "Wrote " << path << " (" << kFrames << " stereo frames at " << kSampleRate
                  << " Hz)\n";
    } catch (const std::exception& error) {
        std::cerr << "Render failed: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
