#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <os/lock.h>
#include <pthread.h>
namespace sys01::probe {
struct Counts {
    std::uint64_t allocation = 0, deallocation = 0, mutex = 0, unfair = 0;
    std::array<void*, 24> allocationStack{};
    int stackSize = 0;
};
// Diagnostic only: one observed thread covers host entry through format return.
void begin() noexcept;
Counts end() noexcept;
void printAllocation(const Counts&);
// Must live in the fixture image: dyld excludes replacement-image bindings
// from interposition so that replacement functions can call the originals.
inline void selfTest() {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    os_unfair_lock unfair = OS_UNFAIR_LOCK_INIT;
    begin();
    void* (*volatile allocate)(std::size_t) = malloc;
    auto* p = allocate(1024);
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
    os_unfair_lock_lock(&unfair);
    os_unfair_lock_unlock(&unfair);
    free(p);
    const auto result = end();
    pthread_mutex_destroy(&mutex);
    std::printf("probe negative control alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                result.allocation, result.deallocation, result.mutex, result.unfair);
    if (result.allocation == 0 || result.deallocation == 0 || result.mutex == 0 ||
        result.unfair == 0) {
        std::fprintf(stderr, "native interposition negative control failed\n");
        std::exit(1);
    }
}
} // namespace sys01::probe
