#pragma once

#include <algorithm>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace sys01 {
#if SYS01_AU_OWNERSHIP_FIXTURE
// Quiescent diagnostic installation; observer only counts the real unlink work.
static void (*registryExitProbe)(AudioUnitPropertyID, std::size_t) = nullptr;
extern "C" __attribute__((visibility("default"))) void
Sys01AUFixtureRegistryExitProbe(void (*hook)(AudioUnitPropertyID, std::size_t)) {
    registryExitProbe = hook;
}
#endif
#if SYS01_AU_OWNERSHIP_FIXTURE
// Diagnostic counts actual frame visits; quiescent installation only.
static void (*registryRemovalProbe)(unsigned) = nullptr;
extern "C" __attribute__((visibility("default"))) void
Sys01AUFixtureRegistryRemovalProbe(void (*hook)(unsigned)) {
    registryRemovalProbe = hook;
}
#endif
#if SYS01_AU_OWNERSHIP_FIXTURE
// Count actual retention reallocations; install only while quiescent.
static void (*registryDemandProbe)(std::size_t) = nullptr;
extern "C" __attribute__((visibility("default"))) void
Sys01AUFixtureRegistryDemandProbe(void (*hook)(std::size_t)) {
    registryDemandProbe = hook;
}
#endif
// Lifetime/order repair, not realtime qualification: the registry access gate
// remains blocking and traversal is host-sized. It never spans a host callback.
// Writers allocate/reclaim; removal acknowledges matching other-thread frames.
class AUPropertyRegistry {
public:
    using Callback = AudioUnitPropertyListenerProc;
    OSStatus add(AudioUnitPropertyID id, Callback callback, void* context) {
        // Invalid registration cannot acquire callback ownership. Reject before
        // the gate, allocation, demand attachment or immutable publication.
        // A null context is valid and remains part of the matching tuple.
        if (callback == nullptr)
            return kAudio_ParamError;
        const std::lock_guard lock(gate);
        if (closed)
            return kAudioUnitErr_Uninitialized;
        auto next = std::make_shared<Entries>();
        // Prepare the complete publication before copying. Copy construction
        // sizes storage to current, forcing append to allocate and free again.
        // All throwing work still precedes demand attachment and publication.
        next->reserve(current->size() + 1);
        next->insert(next->end(), current->begin(), current->end());
        auto entry = std::make_shared<Entry>(id, callback, context);
        next->push_back(entry);
        // A new matching registration can enter while removers wait. Reserve
        // every demand owner's retention before publication, off the reader path.
        for (auto* demand = demands; demand != nullptr; demand = demand->next)
            if (matches(*entry, demand->id, demand->callback, demand->context, demand->specified)) {
#if SYS01_AU_OWNERSHIP_FIXTURE
                if (registryDemandProbe != nullptr &&
                    demand->entries.size() == demand->entries.capacity())
                    registryDemandProbe(demand->entries.size());
#endif
                if (demand->entries.size() == demand->entries.capacity()) {
                    // Grow only full retention, before ANY attachment/count or
                    // publication. Saturate at vector's limit without overflow;
                    // at max_size reserve(size+1) preserves the throwing boundary.
                    const auto size = demand->entries.size();
                    const auto maximum = demand->entries.max_size();
                    const auto grown = size <= maximum / 2 ? size * 2 : maximum;
                    demand->entries.reserve(std::max(size + 1, grown));
                }
            }
        retired.push_back(current);
        for (auto* demand = demands; demand != nullptr; demand = demand->next)
            if (matches(*entry, demand->id, demand->callback, demand->context, demand->specified)) {
                demand->entries.push_back(entry); // reserved; shared_ptr copy cannot throw
                ++entry->waitingRemovals;
            }
        current = std::move(next);
        reclaim();
        return noErr;
    }
    OSStatus remove(AudioUnitPropertyID id, Callback callback, void* context, bool specified) {
        std::unique_lock lock(gate);
        if (closed)
            return noErr;
        const auto caller = std::this_thread::get_id();
        bool inCallback = false, needsAcknowledgement = false;
        for (auto* frame = frames; frame != nullptr; frame = frame->next) {
#if SYS01_AU_OWNERSHIP_FIXTURE
            if (registryRemovalProbe != nullptr)
                registryRemovalProbe(0);
#endif
            inCallback |= frame->thread == caller;
            needsAcknowledgement |= frame->thread != caller && &frame->owner == this &&
                                    matches(*frame->entry, id, callback, context, specified);
        }
        // A callback cannot wait for another callback that may depend on its
        // return. Reject before allocation/withdrawal; failure never authorizes
        // context destruction. Retry outside callbacks after owners are joined.
        // Include callbacks on every SYS01 AU instance in this loaded module:
        // a target on another instance may also depend on the caller's return.
        // Matching ownership remains instance-specific, including withdrawn frames.
        if (inCallback && needsAcknowledgement)
            return kAudioUnitErr_CannotDoInCurrentContext;
        // An absent registration needs no publication or reclamation. Check
        // live frames first: a second remover must still acknowledge withdrawn
        // other-thread entries even when current no longer contains a match.
        // The gate linearizes this return with every add and frame admission.
        const auto publishedMatches =
            std::count_if(current->begin(), current->end(), [&](const auto& entry) {
                return matches(*entry, id, callback, context, specified);
            });
        const bool publishedMatch = publishedMatches != 0;
        if (!needsAcknowledgement && !publishedMatch)
            return noErr;
        // Retain each matching live entry once, including withdrawn entries.
        // Allocate before mutation; demand owners outlive all acknowledged frames.
        Entries awaited;
        // The complete scan above and EVERY writer/admission/exit share this
        // gate. With no matching other-thread frame, retention cannot be needed.
        if (needsAcknowledgement) {
            // The gate serializes EVERY collector. Marks belong only to this
            // preparation scope, never to a waiting demand or reader. Cleanup
            // also runs if vector growth throws; no publication/count changed.
            CollectionMarks marks{awaited};
            for (auto* frame = frames; frame != nullptr; frame = frame->next) {
#if SYS01_AU_OWNERSHIP_FIXTURE
                if (registryRemovalProbe != nullptr)
                    registryRemovalProbe(1);
#endif
                if (frame->thread != caller && &frame->owner == this &&
                    matches(*frame->entry, id, callback, context, specified) &&
                    !frame->entry->retainedForRemoval) {
                    // Retention can throw. Mark only after successful append,
                    // so every marked Entry is owned by the cleanup scope.
                    awaited.push_back(frame->entry);
                    frame->entry->retainedForRemoval = true;
                }
            }
        }
        // A withdrawn-only remover still owns independent acknowledgement
        // demand, but has nothing to withdraw. Do not allocate/publish an
        // equivalent snapshot or reclaim unrelated retired storage for it.
        // The gate keeps this decision atomic with EVERY add and admission.
        if (publishedMatch) {
            auto next = std::make_shared<Entries>();
            // Prepare only surviving slots, before any withdrawal/publication.
            // Counting and copying share the gate, so EVERY writer sees the
            // same immutable current. Removed duplicates need no new storage.
            next->reserve(current->size() - static_cast<std::size_t>(publishedMatches));
            for (const auto& entry : *current)
                if (!matches(*entry, id, callback, context, specified))
                    next->push_back(entry);
            // Finish every throwing allocation before withdrawal. Old storage is
            // retained before publication; even a failed writer cannot strand a reader.
            retired.push_back(current);
            for (const auto& entry : *current)
                if (matches(*entry, id, callback, context, specified))
                    entry->active = false;
            current = std::move(next);
            reclaim();
        }
        // No gate release has occurred since the complete frame scan. New
        // admission cannot race this decision; same-thread ancestors remain
        // alive until they return. No demand or duplicate predicate walk needed.
        if (!needsAcknowledgement)
            return noErr;
        // Publish notification demand under the same gate as frame exit, before
        // wait atomically releases it. Retain demand across spurious wakeups.
        // The scope withdraws demand while the gate is still owned, including
        // exceptional wait exit. No callback can miss its remover's wakeup.
        WaitDemand demand(*this, awaited, id, callback, context, specified);
        acknowledged.wait(lock, [&] {
            // Successful waiting removers own no caller-thread callback frame.
            // Collection retains EVERY matching live Entry, including withdrawn
            // ones; EVERY later matching add attaches before publication. Under
            // this same gate, liveFrames therefore covers the complete predicate
            // without walking unrelated frames or repeated frames of one Entry.
            for (const auto& entry : awaited) {
#if SYS01_AU_OWNERSHIP_FIXTURE
                if (registryRemovalProbe != nullptr)
                    registryRemovalProbe(2);
#endif
                if (entry->liveFrames != 0)
                    return false;
            }
            return true;
        });
        // Reentrant active/ancestor contexts live until their same-thread frames
        // return. Cross-thread removal returns only after matching frames finish.
        return noErr;
    }
    void changed(AudioUnit unit, AudioUnitPropertyID id, AudioUnitScope scope,
                 AudioUnitElement element) {
        std::shared_ptr<Entries> snapshot;
        {
            const std::lock_guard lock(gate);
            if (closed)
                return;
            snapshot = current;
        }
        for (const auto& entry : *snapshot) {
            if (entry->id != id)
                continue;
            Frame frame{*this, entry};
            {
                const std::lock_guard lock(gate);
                if (closed || !entry->active)
                    continue;
                frame.next = frames;
                frame.previous = &frames;
                if (frames != nullptr)
                    frames->previous = &frame.next;
                frames = &frame;
                frame.linked = true;
                ++entry->liveFrames;
            }
            // Frame insertion and active check linearize together with removal.
            // Callback/host reentry runs with no registry or ingress gate held.
            entry->callback(entry->context, unit, id, scope, element);
        }
        // current/retired retain snapshots/nodes; normal reader never deletes last.
    }
    void shutdown() {
        const std::lock_guard lock(gate);
        closed = true;
        for (const auto& entry : *current)
            entry->active = false;
        // Legal teardown excludes/joins all wrapper entries, including complete
        // dispatch frames. Disposal within a callback remains invalid.
        retired.clear();
        current.reset();
    }

private:
    struct Entry {
        Entry(AudioUnitPropertyID i, Callback c, void* x) : id(i), callback(c), context(x) {}
        const AudioUnitPropertyID id;
        const Callback callback;
        void* const context;
        bool retainedForRemoval = false; // gate-owned, temporary collection scope only
        bool active = true;              // gate protects all active/publication/frame access
        std::size_t waitingRemovals = 0; // gate-owned matching removal demand
        std::size_t liveFrames = 0;      // gate-owned admitted frames for this entry
    };
    struct Frame {
        AUPropertyRegistry& owner;
        const std::shared_ptr<Entry>& entry; // immutable enclosing snapshot owns this slot
        const std::thread::id thread = std::this_thread::get_id();
        Frame* next = nullptr;
        Frame** previous = nullptr; // incoming link: owner head or live predecessor next
        bool linked = false;
        ~Frame() {
            if (!linked)
                return;
            std::unique_lock lock(owner.gate);
            // Admission and every exit update incoming links under the same
            // gate. The predecessor remains live while its next owns this link;
            // if it exits first, it transfers that link to its own predecessor.
            // No frame-list search, cap, TLS or reclamation on reader return.
            *previous = next;
            if (next != nullptr)
                next->previous = previous;
#if SYS01_AU_OWNERSHIP_FIXTURE
            if (registryExitProbe != nullptr)
                registryExitProbe(entry->id, 0);
#endif
            // An off-callback remover needs every frame of this Entry to return.
            // Intermediate nested/overlapping exits cannot satisfy it. Admission,
            // withdrawal, demand and this final-exit transition share the gate.
            // Callback-origin waits with other-thread matches are rejected, so
            // no waiting demand may ignore a remaining same-thread frame here.
            // The final exit still wakes ALL independent matching removers.
            const bool notify = --entry->liveFrames == 0 && entry->waitingRemovals != 0;
            // Decide under the gate, then release it before entering the OS
            // broadcast. A remover either sees the completed unlink or waits
            // atomically before this notification; no acknowledgement is lost.
            // Legal wrapper teardown joins complete dispatch, so owner and its
            // condition variable remain alive through this destructor's return.
            lock.unlock();
            if (notify)
                owner.acknowledged.notify_all();
        }
    };
    static bool matches(const Entry& entry, AudioUnitPropertyID id, Callback callback,
                        void* context, bool specified) {
        return entry.id == id && entry.callback == callback &&
               (!specified || entry.context == context);
    }
    using Entries = std::vector<std::shared_ptr<Entry>>;
    struct CollectionMarks {
        Entries& entries;
        ~CollectionMarks() {
            for (const auto& entry : entries)
                entry->retainedForRemoval = false;
        }
    };
    struct WaitDemand {
        AUPropertyRegistry& owner;
        Entries& entries;
        const AudioUnitPropertyID id;
        const Callback callback;
        void* const context;
        const bool specified;
        WaitDemand* next;
        WaitDemand** previous;
        WaitDemand(AUPropertyRegistry& o, Entries& e, AudioUnitPropertyID i, Callback c, void* x,
                   bool s)
            : owner(o), entries(e), id(i), callback(c), context(x), specified(s), next(o.demands),
              previous(&o.demands) {
            for (const auto& entry : entries)
                ++entry->waitingRemovals;
            if (next != nullptr)
                next->previous = &this->next;
            owner.demands = this;
        }
        ~WaitDemand() {
            // All paths retain the gate, including condition wait exceptions.
            *previous = next;
            if (next != nullptr)
                next->previous = previous;
            for (const auto& entry : entries)
                --entry->waitingRemovals;
        }
    };
    void reclaim() {
        std::erase_if(retired, [](const auto& snapshot) { return snapshot.use_count() == 1; });
    }
    // One ownership domain for all SYS01 AU instances in this loaded module.
    // Reuse the intrusive live stack frames, not TLS or a registration table.
    // EVERY writer/capture/admission/exit uses this gate; callbacks run unlocked.
    // Module unload, like instance teardown, excludes and joins COMPLETE entries.
    inline static std::mutex gate;
    std::condition_variable acknowledged;
    WaitDemand* demands = nullptr;         // stack owners, gate-held writer access only
    inline static Frame* frames = nullptr; // module-wide live frames, protected by gate
    std::shared_ptr<Entries> current = std::make_shared<Entries>();
    std::vector<std::shared_ptr<Entries>> retired;
    bool closed = false;
};
} // namespace sys01
