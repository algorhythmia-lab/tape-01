# Immutable native telemetry / Sprint 04 input

`sys01::TelemetrySnapshot` is an in-process, trivially copyable block result,
not the persistent state schema or an ABI promised to hosts. It carries a
monotonic rendered-block sequence (uint64), frames, sanitized sample rate,
cumulative rejections and dropped-snapshot count (uint32), and host::Status.
Status includes actual DSP domain/word/preview/running/brake, per-block clipped
and batch-rejected flags, aggregate peaks and **two separately measured** input
and post-limiter output channel peaks. The meter arrays index L/R; input peaks
are sanitized external input before processing, output peaks are the rendered
samples after trim/bypass/limit. No aggregate value is duplicated to claim stereo
metering, and no RMS/held clip/host CPU/clock/latency data is implied.

One audio producer publishes immutable complete values through an eight-usable-slot
SPSC queue. Full queue drops the new snapshot and increments an audio-owned drop
counter; audio never overwrites unread slots, allocates, locks, spins or waits.
One message-thread consumer calls readTelemetry once per 30 Hz timer tick. It
captures a write boundary, consumes at most eight snapshots and returns the latest
at that boundary; it returns its previous value if no new snapshot exists. Initial
block zero means no completed audio block. No second consumer may read this queue.
Opening/reopening an editor continues the same message-thread ownership.

A snapshot may be stale after a stalled message thread (queue contains the first
eight blocks after the previous read). The next successful publish reports drops;
there is no overwrite-latest guarantee. Frames and sampleRate define the reported
block duration, not wall-clock age. Closed editors run no telemetry timer; audio
publication remains bounded, and a reopening editor drains backlog then receives
fresh data on its next tick. Sprint 04 must handle stale/initial data, counter wrap,
block peak decay and optional UI peak hold explicitly. Polling never changes DSP.
Host targets still come from declared parameters; MIDI performance input appears
in DSP telemetry without becoming saved host values. Transaction rejection and
telemetry drops have different meanings. Rejections include malformed state,
restore/UI queue overflow and rejected musical batches; drops are display-only.
