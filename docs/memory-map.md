# audio ram plan

the firmware audio contract is 48000 stereo frames/s and 64 frames/block. one planar stereo float tape is owned by the engine. capture and freeze share the same storage without copies.

| reservation | calculation | bytes |
|---|---:|---:|
| stereo tape inside engine | 32768 frames × 2 channels × 4 bytes | 262144 |
| remaining engine state and padding | host measurement | 680 |
| planar input/output | 64 frames × 2 channels × 2 directions × 4 bytes | 1024 |
| double-buffered receive/transmit | 64 frames × 2 channels × 2 buffers × 2 directions × 4 bytes | 2048 |
| total firmware audio aggregate | host measurement | 265896 |

the tape covers about 0.683 s at 48 khz. `config.hpp` guards tape storage at 256 kib. the firmware contract guards engine including tape at 320 kib and the complete audio aggregate at 384 kib. there is no separate unused pcm16 freeze reservation. compile-time assertions repeat on the target abi; host sizes do not establish the device linker map.

keep the aggregate at static duration on the device. initialization clears history exclusively before audio starts; live freeze only inhibits writes. the rp2350 has 520 kb sram shared with stacks, runtime, dma descriptors, tables, and ram-resident code. the earlier strategy's 128 kb history allowance must be reconciled against this implemented 256 kib tape before board release.

the 30% processing headroom target implies a render/transport budget below approximately 0.933 ms per 1.333 ms block. host throughput measurements cannot certify that target; measure every scene, transitions, transport conversion, and flash persistence on the actual board.
