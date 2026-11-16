// Native Max SDK adapter. The musical implementation lives exclusively in DSP.
#include "algorhythmia/engine.hpp"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>

namespace {
constexpr std::size_t chunkFrames = 64;
constexpr std::uint32_t toBits(float value) noexcept {
    return std::bit_cast<std::uint32_t>(value);
}

struct AdapterState {
    algorhythmia::Engine engine;
    float sampleRate = 0;
    std::array<float, chunkFrames> inputL{};
    std::array<float, chunkFrames> inputR{};
    std::array<float, chunkFrames> outputL{};
    std::array<float, chunkFrames> outputR{};

    // Messages never access Engine, so DSP graph reinitialization cannot race
    // an Engine setter. Each independently published control is latest-wins.
    std::array<std::atomic<std::uint32_t>, 4> controls{toBits(0.5F), toBits(0.5F), toBits(0.0F),
                                                       toBits(0.0F)};
    std::atomic<std::uint32_t> scene{0};
};

static_assert(std::atomic<std::uint32_t>::is_always_lock_free);

struct MaxObject {
    t_pxobject object;
    AdapterState* state;
};

t_class* externalClass = nullptr;

void publish(MaxObject* self, std::size_t index, double value) noexcept {
    if (self->state != nullptr && std::isfinite(value)) {
        const auto normalized = static_cast<float>(std::clamp(value, 0.0, 1.0));
        self->state->controls[index].store(toBits(normalized), std::memory_order_relaxed);
    }
}

void fluxMessage(MaxObject* self, double value) {
    publish(self, 0, value);
}
void biasMessage(MaxObject* self, double value) {
    publish(self, 1, value);
}
void driveMessage(MaxObject* self, double value) {
    publish(self, 2, value);
}
void stateMessage(MaxObject* self, double value) {
    publish(self, 3, value);
}
void sceneMessage(MaxObject* self, long value) {
    if (self->state != nullptr && value >= 0 && value <= 2) {
        self->state->scene.store(static_cast<std::uint32_t>(value), std::memory_order_relaxed);
    }
}

float fromMaxSample(double sample) noexcept {
    constexpr auto largest = static_cast<double>(std::numeric_limits<float>::max());
    return std::isfinite(sample) && sample <= largest && sample >= -largest
               ? static_cast<float>(sample)
               : 0.0F;
}

void perform64(MaxObject* self, t_object*, double** inputs, long, double** outputs, long,
               long frames, long, void*) {
    auto& state = *self->state;
    constexpr std::array parameterIds{
        algorhythmia::ParameterId::Flux, algorhythmia::ParameterId::Bias,
        algorhythmia::ParameterId::Drive, algorhythmia::ParameterId::State};
    for (std::size_t index = 0; index < parameterIds.size(); ++index) {
        const auto bits = state.controls[index].load(std::memory_order_relaxed);
        state.engine.setParameter(parameterIds[index], std::bit_cast<float>(bits));
    }
    state.engine.setScene(
        static_cast<algorhythmia::SceneId>(state.scene.load(std::memory_order_relaxed)));

    // Bounded stack and instance storage for every host vector size. Copy both
    // inputs before writing either output: Max may reuse input signal buffers.
    for (long offset = 0; offset < frames;) {
        const auto count = std::min(chunkFrames, static_cast<std::size_t>(frames - offset));
        for (std::size_t index = 0; index < count; ++index) {
            const auto hostIndex = offset + static_cast<long>(index);
            state.inputL[index] = fromMaxSample(inputs[0][hostIndex]);
            state.inputR[index] = fromMaxSample(inputs[1][hostIndex]);
        }
        state.engine.process(state.inputL.data(), state.inputR.data(), state.outputL.data(),
                             state.outputR.data(), count);
        for (std::size_t index = 0; index < count; ++index) {
            const auto hostIndex = offset + static_cast<long>(index);
            outputs[0][hostIndex] = static_cast<double>(state.outputL[index]);
            outputs[1][hostIndex] = static_cast<double>(state.outputR[index]);
        }
        offset += static_cast<long>(count);
    }
}

void dsp64(MaxObject* self, t_object* chain, short*, double sampleRate, long, long) {
    // Max has stopped this object's perform routine while compiling its DSP
    // graph. UI callbacks only publish to AdapterState atomics above.
    const auto rate = static_cast<float>(sampleRate);
    if (self->state->sampleRate != rate) {
        self->state->engine.init(rate);
        self->state->sampleRate = rate;
    }
    object_method(chain, gensym("dsp_add64"), self, perform64, 0, nullptr);
}

void freeObject(MaxObject* self) {
    dsp_free(&self->object);
    delete self->state; // Lifecycle operation only, never an audio callback.
}

void* newObject() {
    auto* self = static_cast<MaxObject*>(object_alloc(externalClass));
    if (self == nullptr) {
        return nullptr;
    }
    self->state = nullptr;
    dsp_setup(&self->object, 2);
    self->state = new (std::nothrow) AdapterState{};
    if (self->state == nullptr) {
        object_error(reinterpret_cast<t_object*>(self), "Unable to allocate DSP instance");
        object_free(self);
        return nullptr;
    }
    outlet_new(reinterpret_cast<t_object*>(self), "signal");
    outlet_new(reinterpret_cast<t_object*>(self), "signal");
    return self;
}
} // namespace

extern "C" C74_EXPORT void ext_main(void*) {
    auto* objectClass =
        class_new("algorhythmia~", reinterpret_cast<method>(newObject),
                  reinterpret_cast<method>(freeObject), sizeof(MaxObject), nullptr, A_NOTHING, 0);
    class_addmethod(objectClass, reinterpret_cast<method>(dsp64), "dsp64", A_CANT, 0);
    class_addmethod(objectClass, reinterpret_cast<method>(fluxMessage), "flux", A_FLOAT, 0);
    class_addmethod(objectClass, reinterpret_cast<method>(biasMessage), "bias", A_FLOAT, 0);
    class_addmethod(objectClass, reinterpret_cast<method>(driveMessage), "drive", A_FLOAT, 0);
    class_addmethod(objectClass, reinterpret_cast<method>(stateMessage), "state", A_FLOAT, 0);
    class_addmethod(objectClass, reinterpret_cast<method>(sceneMessage), "scene", A_LONG, 0);
    class_dspinit(objectClass);
    class_register(CLASS_BOX, objectClass);
    externalClass = objectClass;
}
