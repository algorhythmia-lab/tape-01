# RP2350 firmware scaffold

The host build's `firmware_contract` executable checks the 48 kHz stereo / 64-frame contract and statically allocated storage. Run it after a normal root build:

```sh
./build/apps/firmware_stub/firmware_contract
```

one stereo float tape inside the engine uses 256 kib; engine, planar float i/o and double-buffered transport fit inside the 384 kib total audio ceiling. freeze inhibits writes to that same tape without copying storage. Dimensions come from `algorhythmia/config.hpp`. The [memory plan](../../docs/memory-map.md) separates planned audio storage from overall system SRAM.

## Pico SDK cross-build

Install an ARM bare-metal GCC toolchain with C++20 support and supply an official [Pico SDK 2.x checkout](https://github.com/raspberrypi/pico-sdk), including the SDK's required submodules. Configure this subdirectory independently so the SDK selects its toolchain before CMake's `project()` call:

```sh
cmake -S apps/firmware_stub -B build-pico \
  -DPICO_SDK_PATH=/absolute/path/to/pico-sdk \
  -DPICO_BOARD=pico2 -DPICO_PLATFORM=rp2350-arm-s \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-pico --parallel
```

This selects the RP2350 Cortex-M33 reference platform. No custom board pinout is implied by `pico2`. The source guards processor architecture, float size, memory budgets, sample rate and channels, and the engine requires native lock-free 32-bit atomic loads/stores. The desktop toolchain is not sufficient to validate this cross-build. See [Pico SDK documentation](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html).

`src/main.cpp` owns a static `FirmwareState`, renders one block as a link/bring-up probe, and then idles. It does not generate an audio clock or audible hardware output. ADC/DAC selection, PIO programs, pin assignment, DMA block ownership and interrupt scheduling await the board integration. Reserve and measure stack/runtime space as well as the audio aggregate; inspect the target linker map and worst-case render time before claiming 48 kHz real-time operation.
