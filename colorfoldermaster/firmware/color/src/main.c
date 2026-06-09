#include "audio/audio.h"
#include "synth/synth.h"
#include "drums/drums.h"
#include "fm/fm.h"
#include "pulse/pulse_music.h"

cli();
ntsc_init();          // your existing NTSC setup
audio_init();
synth_init();
fm_init();
pulse_music_init();
sei();

for (;;) {
    // your existing frame / VRAM logic
    pulse_music_update();

    float mix = 0.0f;
    mix += synth_update_mix();
    mix += drums_update_mix();
    mix += fm_update_mix();

    if (mix > 1.0f) mix = 1.0f;
    if (mix < -1.0f) mix = -1.0f;
    audio_sample = (uint8_t)(mix * 120.0f + 128);
}
