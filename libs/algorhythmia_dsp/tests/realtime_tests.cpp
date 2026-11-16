#include <algorhythmia/engine.hpp>
#include <algorhythmia/host/processor.hpp>
#include <algorhythmia/parameters.hpp>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <new>
#include <thread>

#if defined(_MSC_VER)
#include <malloc.h>
#endif

namespace {

// This executable is separate from doctest so its allocation hooks do not count
// assertion/reporting allocations. malloc/free are not interposed: Unix CTest
// also checks the DSP archive for direct references to runtime allocators.
std::atomic<bool> allocationProbeArmed{false};
std::atomic<std::size_t> allocationCount{0};
std::atomic<std::size_t> deallocationCount{0};

void noteAllocation() noexcept {
    if (allocationProbeArmed.load(std::memory_order_relaxed)) {
        allocationCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void noteDeallocation(void* pointer) noexcept {
    if (pointer != nullptr && allocationProbeArmed.load(std::memory_order_relaxed)) {
        deallocationCount.fetch_add(1, std::memory_order_relaxed);
    }
}

[[nodiscard]] void* allocate(std::size_t bytes) {
    noteAllocation();
    if (auto* pointer = std::malloc(bytes == 0 ? 1 : bytes)) {
        return pointer;
    }
    throw std::bad_alloc{};
}

[[nodiscard]] void* allocateAligned(std::size_t bytes, std::size_t alignment) {
    noteAllocation();
    if (bytes > std::numeric_limits<std::size_t>::max() - alignment) {
        throw std::bad_alloc{};
    }
    const auto rounded = ((bytes == 0 ? 1 : bytes) + alignment - 1) / alignment * alignment;
#if defined(_MSC_VER)
    auto* pointer = _aligned_malloc(rounded, alignment);
#else
    auto* pointer = std::aligned_alloc(alignment, rounded);
#endif
    if (pointer != nullptr) {
        return pointer;
    }
    throw std::bad_alloc{};
}

void deallocate(void* pointer) noexcept {
    noteDeallocation(pointer);
    std::free(pointer);
}

void deallocateAligned(void* pointer) noexcept {
    noteDeallocation(pointer);
#if defined(_MSC_VER)
    _aligned_free(pointer);
#else
    std::free(pointer);
#endif
}

[[nodiscard]] bool validSample(float sample) noexcept {
    return std::isfinite(sample) && std::fpclassify(sample) != FP_SUBNORMAL;
}

} // namespace

void* operator new(std::size_t bytes) {
    return allocate(bytes);
}
void* operator new[](std::size_t bytes) {
    return allocate(bytes);
}
void operator delete(void* pointer) noexcept {
    deallocate(pointer);
}
void operator delete[](void* pointer) noexcept {
    deallocate(pointer);
}
void operator delete(void* pointer, std::size_t) noexcept {
    deallocate(pointer);
}
void operator delete[](void* pointer, std::size_t) noexcept {
    deallocate(pointer);
}
void* operator new(std::size_t bytes, std::align_val_t alignment) {
    return allocateAligned(bytes, static_cast<std::size_t>(alignment));
}
void* operator new[](std::size_t bytes, std::align_val_t alignment) {
    return allocateAligned(bytes, static_cast<std::size_t>(alignment));
}
void operator delete(void* pointer, std::align_val_t) noexcept {
    deallocateAligned(pointer);
}
void operator delete[](void* pointer, std::align_val_t) noexcept {
    deallocateAligned(pointer);
}
void operator delete(void* pointer, std::size_t, std::align_val_t) noexcept {
    deallocateAligned(pointer);
}
void operator delete[](void* pointer, std::size_t, std::align_val_t) noexcept {
    deallocateAligned(pointer);
}
void* operator new(std::size_t bytes, const std::nothrow_t&) noexcept {
    try {
        return allocate(bytes);
    } catch (...) {
        return nullptr;
    }
}
void* operator new[](std::size_t bytes, const std::nothrow_t& tag) noexcept {
    return ::operator new(bytes, tag);
}
void operator delete(void* pointer, const std::nothrow_t&) noexcept {
    deallocate(pointer);
}
void operator delete[](void* pointer, const std::nothrow_t&) noexcept {
    deallocate(pointer);
}
void* operator new(std::size_t bytes, std::align_val_t alignment, const std::nothrow_t&) noexcept {
    try {
        return allocateAligned(bytes, static_cast<std::size_t>(alignment));
    } catch (...) {
        return nullptr;
    }
}
void* operator new[](std::size_t bytes, std::align_val_t alignment,
                     const std::nothrow_t& tag) noexcept {
    return ::operator new(bytes, alignment, tag);
}
void operator delete(void* pointer, std::align_val_t, const std::nothrow_t&) noexcept {
    deallocateAligned(pointer);
}
void operator delete[](void* pointer, std::align_val_t, const std::nothrow_t&) noexcept {
    deallocateAligned(pointer);
}

int main() {
    using algorhythmia::Engine;
    using algorhythmia::ParameterId;
    using algorhythmia::SceneId;
    constexpr std::size_t blockSize = 128;
    std::array<float, blockSize> left{};
    std::array<float, blockSize> right{};
    Engine engine;
    std::array<float, blockSize> inputL{}, inputR{};
    for (std::size_t i = 0; i < blockSize; ++i) {
        inputL[i] = static_cast<float>(i) / 128.0F - 0.5F;
        inputR[i] = -inputL[i];
    }

    // Only the core operations execute while the hook is armed. This includes
    // reset and pending control changes as well as the hot rendering loop.
    allocationProbeArmed.store(true, std::memory_order_relaxed);
    engine.init(48'000.0F);
    engine.setParameter(ParameterId::Drive, 1.0F);
    for (std::uint32_t block = 0; block < 4000; ++block) {
        const auto position = static_cast<float>(block % 101U) / 100.0F;
        engine.setParameter(ParameterId::Flux, position);
        engine.setParameter(ParameterId::Bias, 1.0F - position);
        engine.setParameter(ParameterId::State, static_cast<float>(block % 2U));
        engine.setScene(static_cast<SceneId>(block % 3U));
        engine.setButton(ParameterId::Drive, (block % 200U) < 120U);
        engine.setButton(ParameterId::State, (block % 1000U) < 800U);
        algorhythmia::CommitSnapshot snapshot;
        static_cast<void>(engine.takeCommitRequest(snapshot));
        static_cast<void>(engine.telemetry());
        engine.process(inputL.data(), inputR.data(), left.data(), right.data(), left.size());
    }
    algorhythmia::host::Processor desktop;
    desktop.prepare(48000);
    static_cast<void>(desktop.restore(algorhythmia::host::presets[1].state));
    for (std::uint32_t block = 0; block < 1000; ++block) {
        const std::array events{
            algorhythmia::host::Event{1, algorhythmia::host::EventKind::Parameter,
                                      algorhythmia::host::Id::Flux,
                                      static_cast<float>(block % 100) / 100},
            algorhythmia::host::Event{63, algorhythmia::host::EventKind::Parameter,
                                      algorhythmia::host::Id::Bypass,
                                      static_cast<float>(block % 2)}};
        static_cast<void>(desktop.process(inputL.data(), inputR.data(), left.data(), right.data(),
                                          blockSize, events));
        if (block % 100 == 0)
            static_cast<void>(desktop.restore(algorhythmia::host::presets[block % 3 + 1].state));
    }
    allocationProbeArmed.store(false, std::memory_order_relaxed);
    if (allocationCount.load() != 0 || deallocationCount.load() != 0) {
        std::fprintf(stderr, "DSP performed %zu allocations and %zu deallocations\n",
                     allocationCount.load(), deallocationCount.load());
        return EXIT_FAILURE;
    }

    engine.init(48'000.0F);
    engine.setParameter(ParameterId::Drive, 1.0F);
    std::atomic<bool> running{true};
    std::atomic<bool> started{false};
    std::thread controls([&] {
        std::uint32_t state = 0x12345678U;
        do {
            state = state * 1664525U + 1013904223U;
            const auto value = static_cast<float>(state & 0xffffU) / 65535.0F;
            engine.setParameter(ParameterId::Flux, value);
            engine.setParameter(ParameterId::Bias, 1.0F - value);
            engine.setParameter(ParameterId::Drive, value >= 0.1F ? 1.0F : 0.0F);
            engine.setParameter(ParameterId::State, static_cast<float>(state & 1U));
            engine.setScene(static_cast<SceneId>((state >> 16U) % 3U));
            engine.setButton(ParameterId::Drive, (state & 8U) != 0);
            engine.setButton(ParameterId::State, (state & 16U) != 0);
            static_cast<void>(engine.telemetry());
            algorhythmia::CommitSnapshot snapshot;
            static_cast<void>(engine.takeCommitRequest(snapshot));
            started.store(true, std::memory_order_release);
        } while (running.load(std::memory_order_relaxed));
    });
    while (!started.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    // This same stress run can be built with ThreadSanitizer to check races.
    // No particular output sequence is expected from asynchronous controls.
    bool valid = true;
    for (std::size_t block = 0; block < 10'000; ++block) {
        engine.process(inputL.data(), inputR.data(), left.data(), right.data(), left.size());
        for (std::size_t frame = 0; frame < blockSize; ++frame) {
            valid = valid && validSample(left[frame]) && validSample(right[frame]);
        }
    }
    running.store(false, std::memory_order_relaxed);
    controls.join();
    if (!valid) {
        std::fputs("Concurrent controls produced nonfinite or subnormal output\n", stderr);
        return EXIT_FAILURE;
    }
    std::puts("Realtime checks passed: no C++ heap traffic; concurrent control stress passed.");
    return EXIT_SUCCESS;
}
