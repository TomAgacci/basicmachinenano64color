#pragma once
#include <stdint.h>

#define N_VOICES 4

typedef struct {
    float attack, decay, sustain, release;
    float level;
    uint8_t state; // 0=off,1=A,2=D,3=S,4=R
} env_t;

typedef struct {
    uint32_t phase;
    uint32_t inc;
    float cutoff;
    float resonance;
    float z1,z2,z3,z4;

    uint8_t waveform;   // 0=ram,1=saw,2=square,3=noise
    uint32_t noise;

    float lfo_phase;
    float lfo_rate;
    float lfo_depth_cutoff;
    float lfo_depth_pitch;

    env_t env;
    uint8_t active;
} voice_t;

void synth_init(void);
void synth_note_on(uint8_t note, uint8_t vel);
void synth_note_off(uint8_t note);
float synth_update_mix(void);
