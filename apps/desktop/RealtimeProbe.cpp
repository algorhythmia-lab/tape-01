#include "RealtimeProbe.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <execinfo.h>
#include <os/lock.h>
#include <pthread.h>

namespace {
// Avoid TLS bootstrap inside dyld allocator interposition. One diagnostic
// scope at a time; ignore allocations/locks on every other thread.
std::atomic<pthread_t> observedThread{nullptr};
sys01::probe::Counts counts;
bool observing() noexcept {
    const auto thread = observedThread.load(std::memory_order_acquire);
    return thread != nullptr && pthread_equal(thread, pthread_self());
}
void allocation() {
    if (!observing())
        return;
    if (counts.allocation++ == 0) {
        // Diagnostic capture only. Exclude the unwinder's own work from native
        // counts; this probe is never linked into release profiling.
        const auto thread = observedThread.exchange(nullptr, std::memory_order_acq_rel);
        counts.stackSize = backtrace(counts.allocationStack.data(),
                                     static_cast<int>(counts.allocationStack.size()));
        observedThread.store(thread, std::memory_order_release);
    }
}
void* probeMalloc(std::size_t n) {
    allocation();
    return malloc(n);
}
void* probeCalloc(std::size_t n, std::size_t size) {
    allocation();
    return calloc(n, size);
}
void* probeRealloc(void* p, std::size_t n) {
    if (observing()) {
        allocation();
        if (p)
            ++counts.deallocation;
    }
    return realloc(p, n);
}
void probeFree(void* p) {
    if (observing() && p)
        ++counts.deallocation;
    free(p);
}
int probeMutex(pthread_mutex_t* p) {
    if (observing())
        ++counts.mutex;
    return pthread_mutex_lock(p);
}
void probeUnfair(os_unfair_lock_t p) {
    if (observing())
        ++counts.unfair;
    os_unfair_lock_lock(p);
}
#define SYS01_INTERPOSE(replacement, original)                                                     \
    __attribute__((used)) const struct {                                                           \
        const void* replace;                                                                       \
        const void* original;                                                                      \
    } interpose_##original __attribute__((section("__DATA,__interpose"))) = {                      \
        reinterpret_cast<const void*>(&replacement), reinterpret_cast<const void*>(&original)}
SYS01_INTERPOSE(probeMalloc, malloc);
SYS01_INTERPOSE(probeCalloc, calloc);
SYS01_INTERPOSE(probeRealloc, realloc);
SYS01_INTERPOSE(probeFree, free);
SYS01_INTERPOSE(probeMutex, pthread_mutex_lock);
SYS01_INTERPOSE(probeUnfair, os_unfair_lock_lock);
} // namespace
namespace sys01::probe {
void begin() noexcept {
    counts = {};
    observedThread.store(pthread_self(), std::memory_order_release);
}
void printAllocation(const Counts& result) {
    for (int i = 0; i < result.stackSize; ++i) {
        Dl_info info{};
        if (dladdr(result.allocationStack[static_cast<std::size_t>(i)], &info))
            std::printf("  native allocation stack %d: %s +0x%llx %s\n", i,
                        info.dli_fname ? info.dli_fname : "unknown",
                        static_cast<unsigned long long>(
                            reinterpret_cast<std::uintptr_t>(
                                result.allocationStack[static_cast<std::size_t>(i)]) -
                            reinterpret_cast<std::uintptr_t>(info.dli_fbase)),
                        info.dli_sname ? info.dli_sname : "<local symbol>");
    }
}
Counts end() noexcept {
    observedThread.store(nullptr, std::memory_order_release);
    return counts;
}
} // namespace sys01::probe
