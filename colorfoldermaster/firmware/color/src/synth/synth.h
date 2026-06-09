#pragma once
#include <stdint.h>

#define N_VOICES 4

void synth_init(void);
void synth_note_on(uint8_t note, uint8_t vel);
void synth_note_off(uint8_t note);
float synth_update_mix(void);
