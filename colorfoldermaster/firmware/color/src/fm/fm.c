#include <math.h>
#include "fm.h"

typedef struct { float phase,freq,amp; } op_t;
static op_t o[4];

void fm_init(void){
    for(int i=0;i<4;i++){
        o[i].phase=0.0f;
        o[i].freq=(440.0f*(i+1))/44100.0f;
        o[i].amp=0.5f/(i+1);
    }
}

float fm_update_mix(void){
    float m=0.0f;
    for(int i=0;i<4;i++){
        o[i].phase+=o[i].freq;
        if(o[i].phase>6.28318f) o[i].phase-=6.28318f;
        float s=sinf(o[i].phase+m);
        m=s*o[i].amp;
    }
    return m;
}
