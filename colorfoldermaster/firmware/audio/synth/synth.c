#include <math.h>
#include "synth.h"

voice_t voices[N_VOICES];

static float env_step(env_t *e) {
    switch (e->state) {
    case 0: return 0.0f;
    case 1:
        e->level += e->attack;
        if (e->level >= 1.0f) { e->level = 1.0f; e->state = 2; }
        break;
    case 2:
        e->level -= e->decay;
        if (e->level <= e->sustain) { e->level = e->sustain; e->state = 3; }
        break;
    case 3:
        break;
    case 4:
        e->level -= e->release;
        if (e->level <= 0.0f) { e->level = 0.0f; e->state = 0; }
        break;
    }
    return e->level;
}

static inline void env_on(env_t *e)  { e->state = 1; }
static inline void env_off(env_t *e) { if (e->state) e->state = 4; }

static uint32_t note_to_inc(uint8_t note) {
    float f = 440.0f * powf(2.0f, (note - 69) / 12.0f);
    float fs = 44100.0f;
    return (uint32_t)((f / fs) * 4294967296.0f);
}

static inline float osc_wave(voice_t *v) {
    v->phase += v->inc;
    uint8_t ph = v->phase >> 24;

    switch (v->waveform) {
    case 0: {
        float t = ph * 0.0245436926f;
        t += 0.6180339887f;
        return sinf(t);
    }
    case 1:
        return (ph / 128.0f) - 1.0f;
    case 2:
        return (ph < 128) ? 1.0f : -1.0f;
    case 3:
        v->noise ^= v->noise << 13;
        v->noise ^= v->noise >> 17;
        v->noise ^= v->noise << 5;
        return ((v->noise & 0xFF) / 128.0f) - 1.0f;
    }
    return 0.0f;
}

static inline float moog_filter(voice_t *v, float input) {
    float fc = v->cutoff;
    float res = v->resonance;
    float x = input - res * v->z4;

    v->z1 += fc * (x - v->z1);
    v->z2 += fc * (v->z1 - v->z2);
    v->z3 += fc * (v->z2 - v->z3);
    v->z4 += fc * (v->z3 - v->z4);

    return v->z4;
}

static inline void lfo_update(voice_t *v) {
    v->lfo_phase += v->lfo_rate;
    if (v->lfo_phase > 6.28318f) v->lfo_phase -= 6.28318f;
    float l = sinf(v->lfo_phase);

    v->cutoff = 0.2f + l * v->lfo_depth_cutoff;
    if (v->cutoff < 0.01f) v->cutoff = 0.01f;
    if (v->cutoff > 0.9f)  v->cutoff = 0.9f;

    v->inc += (uint32_t)(l * v->lfo_depth_pitch * 1000.0f);
}

static voice_t* alloc_voice(void) {
    for (int i=0;i<N_VOICES;i++)
        if (!voices[i].active) return &voices[i];
    return &voices[0];
}

void synth_init(void) {
    for (int i=0;i<N_VOICES;i++) {
        voice_t *v = &voices[i];
        v->phase = 0;
        v->inc = 0;
        v->cutoff = 0.2f;
        v->resonance = 0.3f;
        v->z1 = v->z2 = v->z3 = v->z4 = 0.0f;
        v->waveform = 0;
        v->noise = 0x12345678;
        v->lfo_phase = 0.0f;
        v->lfo_rate = 0.01f;
        v->lfo_depth_cutoff = 0.1f;
        v->lfo_depth_pitch = 0.0f;
        v->env.attack  = 0.01f;
        v->env.decay   = 0.005f;
        v->env.sustain = 0.6f;
        v->env.release = 0.003f;
        v->env.level   = 0.0f;
        v->env.state   = 0;
        v->active = 0;
    }
}

void synth_note_on(uint8_t note, uint8_t vel) {
    (void)vel;
    voice_t *v = alloc_voice();
    v->inc = note_to_inc(note);
    v->active = 1;
    env_on(&v->env);
}

void synth_note_off(uint8_t note) {
    (void)note;
    for (int i=0;i<N_VOICES;i++)
        env_off(&voices[i].env);
}

float synth_update_mix(void) {
    float mix = 0.0f;
    for (int i=0;i<N_VOICES;i++) {
        voice_t *v = &voices[i];
        if (!v->active && v->env.state == 0) continue;

        lfo_update(v);

        float env = env_step(&v->env);
        if (v->env.state == 0 && env <= 0.0f) {
            v->active = 0;
            continue;
        }

        float osc = osc_wave(v);
        float sig = moog_filter(v, osc);
        mix += sig * env;
    }
    return mix;
}
