#pragma once
#include <stdint.h>

void audio_init(void);
void audio_update(void);

extern volatile uint8_t audio_sample;
