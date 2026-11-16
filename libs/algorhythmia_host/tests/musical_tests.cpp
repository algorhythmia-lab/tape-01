#include "algorhythmia/host/processor.hpp"
#include "algorhythmia/host/state.hpp"

#include <bit>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace algorhythmia::host;
namespace {
void require(bool ok, const char* message) {
    if (!ok)
        throw std::runtime_error(message);
}
struct Change {
    unsigned second;
    Event event;
    int recall = -1;
};
struct Study {
    std::string name;
    State initial;
    bool input = false, hostile = false;
    unsigned seconds = 11;
    std::vector<Change> changes;
};
struct Audio {
    std::vector<float> left, right;
};
void put16(std::ostream& out, std::uint16_t x) {
    out.put(static_cast<char>(x));
    out.put(static_cast<char>(x >> 8));
}
void put32(std::ostream& out, std::uint32_t x) {
    put16(out, static_cast<std::uint16_t>(x));
    put16(out, static_cast<std::uint16_t>(x >> 16));
}
void wav(const std::filesystem::path& path, const Audio& audio, unsigned rate) {
    std::ofstream out(path, std::ios::binary);
    out.exceptions(std::ios::badbit | std::ios::failbit);
    const auto bytes = static_cast<std::uint32_t>(audio.left.size() * 8);
    out.write("RIFF", 4);
    put32(out, 36 + bytes);
    out.write("WAVEfmt ", 8);
    put32(out, 16);
    put16(out, 3); // IEEE float, explicit little endian; analysis precedes attenuation.
    put16(out, 2);
    put32(out, rate);
    put32(out, rate * 8);
    put16(out, 8);
    put16(out, 32);
    out.write("data", 4);
    put32(out, bytes);
    for (std::size_t i = 0; i < audio.left.size(); ++i) {
        put32(out, std::bit_cast<std::uint32_t>(audio.left[i]));
        put32(out, std::bit_cast<std::uint32_t>(audio.right[i]));
    }
}
Audio render(const Study& study, unsigned rate, std::size_t block) {
    Processor p;
    p.prepare(static_cast<float>(rate));
    require(p.restore(study.initial), "study state invalid");
    const auto frames = static_cast<std::size_t>(rate) * study.seconds;
    Audio audio{std::vector<float>(frames), std::vector<float>(frames)};
    std::vector<float> inL(block), inR(block);
    std::size_t change = 0;
    for (std::size_t offset = 0; offset < frames;) {
        std::vector<Event> events;
        while (change < study.changes.size() &&
               static_cast<std::size_t>(study.changes[change].second) * rate == offset) {
            const auto& c = study.changes[change++];
            if (c.recall >= 0)
                require(p.restore(presets[static_cast<std::size_t>(c.recall)].state),
                        "recall failed");
            else
                events.push_back(c.event);
        }
        auto n = std::min(block, frames - offset);
        if (change < study.changes.size())
            n = std::min(n, static_cast<std::size_t>(study.changes[change].second) * rate - offset);
        require(n > 0, "invalid study ordering");
        for (std::size_t i = 0; i < n; ++i) {
            const auto t = static_cast<double>(offset + i) / rate;
            // Each channel is a distinct -18 dBFS peak sine. No normalization.
            inL[i] =
                static_cast<float>(0.12589254117941673 * std::sin(6.283185307179586 * 220 * t));
            inR[i] =
                static_cast<float>(0.12589254117941673 * std::sin(6.283185307179586 * 331 * t));
            if (study.hostile && (offset + i) % 17 == 0) {
                inL[i] = std::numeric_limits<float>::quiet_NaN();
                inR[i] = std::numeric_limits<float>::infinity();
            }
        }
        require(p.process(study.input ? inL.data() : nullptr, study.input ? inR.data() : nullptr,
                          audio.left.data() + offset, audio.right.data() + offset, n, events),
                "study events rejected");
        offset += n;
    }
    for (const auto* channel : {&audio.left, &audio.right})
        for (const auto x : *channel)
            require(std::isfinite(x) && std::abs(x) <= 1, "unbounded/nonfinite study sample");
    return audio;
}
void measure(std::ostream& out, const Study& study, const Audio& audio, unsigned rate,
             std::string_view label, std::size_t first, std::size_t end) {
    unsigned c = 0;
    for (const auto* channel : {&audio.left, &audio.right}) {
        double sum = 0, power = 0, peak = 0, delta = 0;
        std::size_t rails = 0;
        for (auto i = first; i < end; ++i) {
            const double x = (*channel)[i];
            sum += x;
            power += x * x;
            peak = std::max(peak, std::abs(x));
            if (i > 0)
                delta = std::max(delta, std::abs(x - (*channel)[i - 1]));
            rails += std::abs(x) >= 1;
        }
        const auto n = static_cast<double>(end - first);
        out << study.name << ',' << rate << ',' << c++ << ',' << label << ',' << first << ',' << end
            << ',' << peak << ',' << std::sqrt(power / n) << ',' << sum / n << ',' << delta << ','
            << rails << ",0\n";
    }
}
std::vector<Study> studies() {
    std::vector<Study> result;
    for (unsigned domain = 0; domain < 3; ++domain)
        for (unsigned flux = 0; flux < 2; ++flux)
            for (unsigned bias = 0; bias < 2; ++bias)
                for (unsigned state = 0; state < 2; ++state) {
                    Study s;
                    s.name = "extreme-d" + std::to_string(domain) + "-f" + std::to_string(flux) +
                             "-b" + std::to_string(bias) + "-s" + std::to_string(state);
                    s.initial.values = {static_cast<float>(flux),
                                        static_cast<float>(bias),
                                        1,
                                        static_cast<float>(state),
                                        static_cast<float>(domain) / 2,
                                        .75F,
                                        0};
                    s.input = domain == 2;
                    result.push_back(s);
                }
    for (unsigned i = 0; i < presets.size(); ++i) {
        result.push_back({"factory-" + std::to_string(i), presets[i].state, i == 3, false, 11, {}});
        Study recall{"recall-" + std::to_string(i), presets[i].state, i == 3, false, 11, {}};
        for (const auto& [second, program] :
             {std::pair{5U, static_cast<int>(i)}, {7U, 0}, {8U, static_cast<int>(i)}})
            recall.changes.push_back({second, {}, program});
        result.push_back(recall);
    }
    for (unsigned domain = 0; domain < 3; ++domain) {
        Study s{"performance-d" + std::to_string(domain),
                presets[domain + 1].state,
                true,
                false,
                17,
                {}};
        const auto parameter = [&](unsigned second, Id id, float value) {
            s.changes.push_back({second, {0, EventKind::Parameter, id, value}, -1});
        };
        parameter(1, Id::Flux, 0);
        parameter(2, Id::Flux, 1);
        parameter(3, Id::Bias, 0);
        parameter(4, Id::Bias, 1);
        parameter(5, Id::State, 1);
        s.changes.push_back({6, {0, EventKind::Brake, Id::Flux, 1}, -1});
        s.changes.push_back({7, {0, EventKind::Brake, Id::Flux, 0}, -1});
        parameter(8, Id::Drive, 0);
        parameter(9, Id::Drive, 1);
        parameter(10, Id::Output, 1);
        parameter(11, Id::Output, .75F);
        parameter(12, Id::Bypass, 1);
        parameter(13, Id::Bypass, 0);
        parameter(14, Id::Domain, static_cast<float>((domain + 1) % 3) / 2);
        parameter(15, Id::Domain, static_cast<float>((domain + 2) % 3) / 2);
        parameter(16, Id::Domain, static_cast<float>(domain) / 2);
        result.push_back(s);
    }
    unsigned i = 0;
    for (float flux : {.25F, .494F, .495F, .5F, .505F, .506F, .75F}) {
        Study s{"speed-" + std::to_string(i++), presets[3].state, true, false, 11, {}};
        s.changes.push_back({1, {0, EventKind::Parameter, Id::Flux, flux}, -1});
        result.push_back(s);
    }
    result.push_back({"transform-silent", presets[3].state, false, false, 11, {}});
    result.push_back({"transform-hostile", presets[3].state, true, true, 11, {}});
    return result;
}
} // namespace
int main(int argc, char** argv) {
    try {
        require(argc <= 4, "usage: sys01_musical_tests [output-directory [sample-rate [case]]]");
        const unsigned rate = argc > 2 ? static_cast<unsigned>(std::stoul(argv[2])) : 48000;
        require(rate == 44100 || rate == 48000 || rate == 88200 || rate == 96000 ||
                    rate == 176400 || rate == 192000,
                "choose a declared rate");
        std::ofstream metrics, recipes;
        std::filesystem::path folder;
        if (argc > 1) {
            folder = argv[1];
            std::filesystem::create_directories(folder);
            metrics.exceptions(std::ios::badbit | std::ios::failbit);
            recipes.exceptions(std::ios::badbit | std::ios::failbit);
            metrics.open(folder / "metrics.csv");
            recipes.open(folder / "timeline.csv");
            metrics << std::setprecision(12)
                    << "case,rate,channel,window,first_frame,end_frame,peak,rms,dc,max_delta,"
                       "rail_samples,nonfinite\n";
            recipes << "case,input,hostile,seconds,second,kind,id,value,recall_program\n";
        }
        const auto all = studies();
        unsigned rendered = 0;
        for (const auto& s : all) {
            if (argc == 4 && s.name != argv[3])
                continue;
            ++rendered;
            const auto audio = render(s, rate, 511);
            // Independent callback partition; catches accidental block-dependent behavior.
            const auto split = render(s, rate, 127);
            require(audio.left == split.left && audio.right == split.right,
                    "callback partition changes musical behavior");
            if (s.name == "transform-silent" || s.name == "factory-0")
                require(std::all_of(audio.left.begin(), audio.left.end(),
                                    [](float x) { return x == 0; }) &&
                            std::all_of(audio.right.begin(), audio.right.end(),
                                        [](float x) { return x == 0; }),
                        "empty tape/paused initial state leaked sound");
            if (!folder.empty()) {
                wav(folder / (s.name + ".wav"), audio, rate);
                const auto state = encode(s.initial);
                std::ofstream bytes(folder / (s.name + ".sys1"), std::ios::binary);
                bytes.exceptions(std::ios::badbit | std::ios::failbit);
                bytes.write(reinterpret_cast<const char*>(state.data()), state.size());
                measure(metrics, s, audio, rate, "settled-10s", rate,
                        static_cast<std::size_t>(rate) * 11);
                for (unsigned second = 0; second < s.seconds; ++second)
                    measure(metrics, s, audio, rate, "second-" + std::to_string(second),
                            static_cast<std::size_t>(second) * rate,
                            (static_cast<std::size_t>(second) + 1) * rate);
                recipes << s.name << ',' << s.input << ',' << s.hostile << ',' << s.seconds
                        << ",0,initial,-1,0,-1\n";
                for (const auto& c : s.changes) {
                    recipes << s.name << ',' << s.input << ',' << s.hostile << ',' << s.seconds
                            << ',' << c.second << ',' << static_cast<int>(c.event.kind) << ','
                            << static_cast<int>(c.event.id) << ',' << c.event.value << ','
                            << c.recall << '\n';
                    measure(metrics, s, audio, rate, "transition-" + std::to_string(c.second),
                            static_cast<std::size_t>(c.second) * rate,
                            static_cast<std::size_t>(c.second) * rate + rate / 50);
                }
            }
        }
        require(rendered > 0, "unknown fixture case");
        std::cout
            << "PASS: " << rendered << " musical fixtures at " << rate
            << " Hz; exact 511/127-frame partition equality, finite bounded stereo, "
               "empty transform/paused initialization. Sound acceptance requires listening.\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
