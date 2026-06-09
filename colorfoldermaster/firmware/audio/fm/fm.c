// fm.c
#include <math.h>
#include "fm.h"

#define N_OPS 4

typedef struct {
    float phase;
    float freq;
    float amp;
} op_t;

typedef struct {
    op_t op[N_OPS];
} fm_voice_t;

static fm_voice_t fm;

void fm_init(void) {
    for (int i=0;i<N_OPS;i++) {
        fm.op[i].phase = 0.0f;
        fm.op[i].freq = (440.0f * (i+1)) / 44100.0f;
        fm.op[i].amp  = 0.5f / (i+1);
    }
}

float fm_update_mix(void) {
    float m = 0.0f;
    for (int i=0;i<N_OPS;i++) {
        op_t *o = &fm.op[i];
        o->phase += o->freq;
        if (o->phase > 6.28318f) o->phase -= 6.28318f;
        float s = sinf(o->phase + m);
        m = s * o->amp;
    }
    return m;
}
