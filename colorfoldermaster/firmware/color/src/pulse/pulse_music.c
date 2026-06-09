#include <string.h>
#include <stdlib.h>
#include "pulse_music.h"
#include "synth/synth.h"
#include "drums/drums.h"

typedef struct { uint8_t note,len,vel; } step_t;
static step_t pat[16];
static uint8_t row=0,tick=0,tps=8;

void pulse_music_init(void){
    for(int i=0;i<16;i++) pat[i]=(step_t){0,0,0};
    pat[0]=(step_t){60,4,100};
    pat[4]=(step_t){64,4,100};
    pat[8]=(step_t){67,4,100};
    pat[12]=(step_t){72,4,100};
}

void pulse_music_update(void){
    if(++tick<tps) return;
    tick=0;
    step_t *s=&pat[row];
    if(s->len && s->vel){
        synth_note_on(s->note,s->vel);
        drums_trigger(0,80.0f,0.02f,800);
    }
    row=(row+1)&15;
}

void pulse_music_cmd(const char *s){
    if(strncmp(s,"STEP=",5)!=0) return;
    int st=atoi(s+5);
    const char *n=strstr(s,"NOTE=");
    const char *l=strstr(s,"LEN=");
    const char *v=strstr(s,"VEL=");
    if(!n||!l||!v) return;
    pat[st].note=atoi(n+5);
    pat[st].len =atoi(l+4);
    pat[st].vel =atoi(v+4);
}
