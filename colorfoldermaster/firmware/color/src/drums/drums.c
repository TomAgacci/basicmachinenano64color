#include <math.h>
#include <stdint.h>
#include "drums.h"

typedef struct {
    uint8_t active;
    uint16_t t, length;
    float decay;
    uint32_t noise;
} drum_t;

static drum_t d[4];

static inline float n(drum_t *x){
    x->noise^=x->noise<<13; x->noise^=x->noise>>17; x->noise^=x->noise<<5;
    return ((x->noise&0xFF)/128.0f)-1.0f;
}

void drums_trigger(int id,float p,float dec,unsigned len){
    (void)p;
    drum_t *x=&d[id&3];
    x->active=1; x->t=0; x->length=len; x->decay=dec; x->noise=0x87654321;
}

float drums_update_mix(void){
    float m=0.0f;
    for(int i=0;i<4;i++){
        drum_t *x=&d[i];
        if(!x->active) continue;
        if(x->t++>=x->length){x->active=0;continue;}
        m+=n(x)*expf(-x->decay*x->t);
    }
    return m;
}
