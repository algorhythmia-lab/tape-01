# Build boundaries

`ProjectOptions.cmake` applies C++20, strict warnings as errors, and predictable floating-point options per target. It also provides the optional native ASan/UBSan instrumentation. The DSP disables exceptions and RTTI, and builds position-independent code for desktop linkage only.

The root build is native. Configure `apps/firmware_stub` separately for RP2350: the Pico SDK imports its own ARM toolchain before `project()`. Do not mix host tests and firmware toolchains in one build directory. The SDK is the authority for processor/FPU/ABI flags, so this scaffold does not duplicate its toolchain file.

Test-only doctest sources are pinned to a commit. An offline configure can supply an existing checkout with `-DFETCHCONTENT_SOURCE_DIR_DOCTEST=/absolute/path/to/doctest`. `-DBUILD_TESTING=OFF` needs no downloaded dependency.

For static analysis, configure with `-DALGORHYTHMIA_ENABLE_CLANG_TIDY=ON`. Use clang-tidy compatible with the active compiler's standard-library headers. On macOS, a standalone LLVM tool may additionally require `-DCMAKE_OSX_SYSROOT=/path/reported/by/xcrun`; the local bootstrap was checked using clang-tidy 21 and Apple Clang 21 headers. CI runs matching Clang/clang-tidy 18 on Ubuntu. Formatting uses clang-format 18.
