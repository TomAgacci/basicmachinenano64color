#include <math.h>
#include <stdint.h>
#include "drums.h"

#define N_DRUMS 4

typedef struct {
    uint8_t active;
    uint16_t t;
    uint16_t length;
    float decay;
    uint32_t noise;
} drum_t;

static drum_t drums[N_DRUMS];

static inline float drum_noise(drum_t *d) {
    d->noise ^= d->noise << 13;
    d->noise ^= d->noise >> 17;
    d->noise ^= d->noise << 5;
    return ((d->noise & 0xFF) / 128.0f) - 1.0f;
}

void drums_trigger(int id, float pitch, float decay, unsigned length) {
    (void)pitch;
    drum_t *d = &drums[id & (N_DRUMS-1)];
    d->active = 1;
    d->t = 0;
    d->length = length;
    d->decay = decay;
    d->noise = 0x87654321;
}

float drums_update_mix(void) {
    float mix = 0.0f;
    for (int i=0;i<N_DRUMS;i++) {
        drum_t *d = &drums[i];
        if (!d->active) continue;
        if (d->t++ >= d->length) { d->active = 0; continue; }
        float env = expf(-d->decay * d->t);
        float n = drum_noise(d);
        mix += n * env;
    }
    return mix;
}
