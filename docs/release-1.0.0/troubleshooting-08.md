# SYS 01 / troubleshooting

| Observation | Recovery / evidence to retain |
|---|---|
| No generated sound | Ensure the stereo track is active, host processing is running, RUN is on, OUTPUT is audible, and BYPASS is off. Select magnetic pulse. Some hosts suspend a silent effect track. Record host routing; no instrument-shell support is promised. |
| Transform is silent | Feed stereo audio into the effect, release BRAKE, enable RUN, set FLUX .75 (+1x), and allow empty tape to fill. Center is a real stopped detent; stationary output can decay. |
| BYPASS goes silent or changes level | Bypass uses untrimmed dry stereo input, including silence, while the engine advances. Wet and dry/study levels differ. Lower monitoring/OUTPUT before comparison. |
| LIMIT or harsh high output | Lower OUTPUT and input level. +20 dB can intentionally hard-limit; finite/bounded output is not listening acceptance. MB-04 high-trim DC remains an open investigation. |
| Project sounds different after reopen / undo | Only parameters return. Tape, voices and phase restart empty; performance MIDI is not saved as host targets. Use the DAW's recorded/rendered audio for exact performance recall. |
| Visible dial differs from sound after a rejected batch | Retain rejection count and host/binary details. Send a valid subsequent edit or coherent preset/reset; reset erases tape. Entire malformed/overflowing batches reject, without replay next block. |
| AU automation occurs early | Known P1: ordinary/scheduled offsets collapse to the block boundary. Do not claim timing parity or use a smooth ramp as proof of correct offset. Retain exact bundle/report identity. |
| Dropout / sluggish editor | Record OS/CPU/host/driver/rate/buffer, instance count and editor state. Callback timing and full native locks remain P1; increasing a test buffer is diagnostic, not gate closure. UI/AT/200% host reachability remain unqualified. |
| Plugin fails scan or OS trust | Verify package and complete bundle hashes, architecture and host scan path; preserve the validator/host error. Do not modify bundle contents, strip quarantine or bypass signing checks. Release signing/install evidence is missing. |
| Hash mismatch or failed upgrade | Stop using the changed packet; preserve receipt and unexpected files. Recover the verified previous owned version using INSTALL.md. No successful signed rollback is claimed. |

The source includes `scripts/collect_diagnostics.py --output <new-file> --binary
<executable> --host <exact-host-version>`. It emits local OS/architecture, supplied
host and optional executable hash. Review the supplied text; nothing is uploaded.
Include no private project/audio or credentials by default. No new app installation
or remote support transmission is required by these instructions.
