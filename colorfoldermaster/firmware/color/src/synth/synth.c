#include <math.h>
#include "synth.h"

typedef struct {
    float attack, decay, sustain, release;
    float level;
    uint8_t state;
} env_t;

typedef struct {
    uint32_t phase, inc;
    float cutoff, resonance;
    float z1,z2,z3,z4;
    uint8_t waveform;
    uint32_t noise;
    float lfo_phase, lfo_rate;
    float lfo_depth_cutoff, lfo_depth_pitch;
    env_t env;
    uint8_t active;
} voice_t;

static voice_t v[N_VOICES];

static float env_step(env_t *e) {
    switch(e->state) {
        case 0: return 0.0f;
        case 1: e->level += e->attack; if(e->level>=1.0f){e->level=1.0f;e->state=2;} break;
        case 2: e->level -= e->decay;  if(e->level<=e->sustain){e->level=e->sustain;e->state=3;} break;
        case 3: break;
        case 4: e->level -= e->release; if(e->level<=0.0f){e->level=0.0f;e->state=0;} break;
    }
    return e->level;
}

static inline float osc(voice_t *x) {
    x->phase += x->inc;
    uint8_t ph = x->phase >> 24;
    switch(x->waveform) {
        case 0: return sinf(ph*0.0245436926f + 0.6180339887f);
        case 1: return (ph/128.0f)-1.0f;
        case 2: return (ph<128)?1.0f:-1.0f;
        case 3:
            x->noise ^= x->noise<<13; x->noise ^= x->noise>>17; x->noise ^= x->noise<<5;
            return ((x->noise&0xFF)/128.0f)-1.0f;
    }
    return 0.0f;
}

static inline float moog(voice_t *x, float in) {
    float fc=x->cutoff, r=x->resonance;
    float t=in - r*x->z4;
    x->z1+=fc*(t-x->z1);
    x->z2+=fc*(x->z1-x->z2);
    x->z3+=fc*(x->z2-x->z3);
    x->z4+=fc*(x->z3-x->z4);
    return x->z4;
}

void synth_init(void) {
    for(int i=0;i<N_VOICES;i++){
        v[i].cutoff=0.2f; v[i].resonance=0.3f;
        v[i].waveform=0; v[i].noise=0x12345678;
        v[i].lfo_rate=0.01f; v[i].lfo_depth_cutoff=0.1f;
        v[i].env.attack=0.01f; v[i].env.decay=0.005f;
        v[i].env.sustain=0.6f; v[i].env.release=0.003f;
    }
}

void synth_note_on(uint8_t n, uint8_t vel) {
    (void)vel;
    for(int i=0;i<N_VOICES;i++){
        if(!v[i].active){
            v[i].inc = (uint32_t)((440.0f*powf(2,(n-69)/12.0f))/44100.0f*4294967296.0f);
            v[i].active=1;
            v[i].env.state=1;
            return;
        }
    }
}

float synth_update_mix(void) {
    float m=0.0f;
    for(int i=0;i<N_VOICES;i++){
        if(!v[i].active && v[i].env.state==0) continue;
        float e=env_step(&v[i].env);
        if(v[i].env.state==0 && e<=0.0f){v[i].active=0;continue;}
        m+=moog(&v[i], osc(&v[i])) * e;
    }
    return m;
}
