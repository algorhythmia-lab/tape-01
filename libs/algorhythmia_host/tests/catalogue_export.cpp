#include "algorhythmia/host/catalogue.hpp"

#include <cstdio>

int main() {
    std::puts("# SYS 01 parameter catalogue\n\ngenerated from `host/catalogue.hpp`; do not edit by "
              "hand.\n");
    std::puts("| stable ID | label | normalized default | steps |\n|---|---|---|---|");
    for (const auto& p : algorhythmia::host::catalogue)
        std::printf("| `%s` | %s | %.2f | %u |\n", p.key.data(), p.label.data(),
                    static_cast<double>(p.initial), p.steps);
    std::puts("\nsteps 0 means continuous. all parameters are automatable. DOMAIN uses 0 / 0.5 / "
              "1; binary parameters use 0 / 1. OUTPUT maps -60..+20 dB. STATE is shared across "
              "domains. schema 1.\n");
    std::puts("factory studies (parameter-only; empty captured tape on recall):\n");
    for (const auto& p : algorhythmia::host::presets)
        std::printf("- %s\n", p.name.data());
}
