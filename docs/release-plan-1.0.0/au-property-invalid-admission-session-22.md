# Sprint10 session22: reject invalid AU callback admission

S10-AU-REGISTRY-17 repairs a concrete invalid callback ownership admission: a null
function pointer could become an active Entry and later be called. Sprint10 remains
IN PROGRESS; release NO-GO/development-unqualified. Registry gate BLOCKING; G01,
actual DAW/externalAU, MB-04, immutable prepared resources, nonwaiting acquisition,
EVERY-writer acknowledged reclamation and safe invalid concurrent lifecycle remain open.

All163source+43reference inputs matched Session21 before edits; HEAD unchanged,
no applicable AGENTS; all23 historical inventories unchanged. Complete negative
source/header/export/fixture+executable/generated AU-SDK-VST3/cache/compile/pinned29file
SDK/bundle/report frozen BEFORE repair in evidence/sprint-10-session-22/negative-*.
Unchanged Session21 shipping OFF AU fingerprint
092282ca3d257845f4b31cfbe9652f2f8b5178abcd852dd5beffe73cb388da2a;
binary31b3ca82bc294435dffebdb8fc412dc8d74655377877f31841cd7b741bd09884.
Both public AudioUnitAddPropertyListener and real loaded SDK Lookup add selector
accepted null function pointer status0. BOTH WithUserData and SDK Lookup removal
selectors cleaned the invalid registration successfully; valid null contexts and
ordered duplicates already passed. Separate actual SDK NickName publication called
accepted null and fixture exited SIGSEGV(-11). This is intentionally invalid registration,
not an arbitrary host callback pointer, concurrent lifecycle, DAW or realtime claim.

Only product AUPropertyRegistry.hpp changes: reject callback==nullptr with
kAudio_ParamError(-50) BEFORE gate acquisition/allocation/attachment/publication.
The check reads only the passed function pointer. Context may be null. Valid callback
admission is unchanged; no event loss/cap, duplicate coalescing, payload or gesture change.
No diagnostic hook or dependency is added. Null add after registry shutdown also rejects
without reading current/closed; this does not make destruction concurrent with a call safe.

The complete registry protocol remains: every valid writer, capture, frame admission/exit
uses the same gate. ALL-frame callback-origin other-thread rejection stays before removal
allocation/mutation. Unique CollectionMarks append before mark, gate-held cleanup on success/
exception before publication/demand; EVERY matching valid add reserves EACH demand before
ANY attachment/count/publication. Retained Entry liveFrames predicates cover withdrawn,
newmatching and both independent removers/spurious wakes; WaitDemand gate-held RAII cleanup.
Constant incoming-link unlink/final Entry transition/local notification decision/unlock
before OSbroadcast; complete wrapper/processor/message-owner/CV lifetime; updater stop/clear
before registry shutdown while ContextName wrapper-this lives. Ordered duplicate/survivor
snapshots, nested visibility, reader non-reclamation/writer use_count1 reclaim remain.
Error authorizes no context release; legal teardown joins complete entries including tails.

Pinned generated SDK routes/empty SDK vectors/uncalled accessor/MIDI source-only/GetMutex
null remain. APVTS virtual authority->raw mirror->recursive LockedListeners callback->flags,
regular/final parameter listenerLock/editor synchronized removal, processor size outside/
pointer under/invoke unlocked/final forwarding unlocked, VST3 separate atomic floats/grouped
flags/immutable IDs/shared storage remain. Prepared maps/buffers/DSP scratch/history/callback
locks/serialized offaudio restore retain legal lifecycle exclusion. No gate removal without
replacement publication and acknowledged reclamation across EVERY writer/reader/context.

Preserve org.algorhythmia.sys01/Algr/Sys1,7keys-nativeIDs,4programs-order,schema1exactly40,
parameter-only empty-history recall,10ms smoothing,shared256wholebatch reject/no replay,
8restores,singleaudioowner,formats/editor dirty+7packed FLOAT-GENERATION words/gesture-
undo-recall-visibility. Final local verification and exact identities/flags/commands/exits
are in checkpoint, final-identities, sanitizer-flags and reports. Neutral fixture ABI calls
exercise product rejection without caller nonnull UBSan checks preempting it. Only two
inherited external mutex bootstrap shims exclude instrumentation; product/generated/fixtures
remain separately instrumented. Explicit8MiB SDK worker stacks and complete joins remain
finite fixture conditions. No claim of failure-free history or wholeG01 acceptance.
