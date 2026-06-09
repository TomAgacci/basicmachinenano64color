// pulse_music.c
#include <string.h>
#include <stdlib.h>
#include "pulse_music.h"
#include "synth.h"
#include "drums.h"

typedef struct {
    uint8_t note;
    uint8_t length;
    uint8_t vel;
} step_t;

#define PAT_LEN 16

static step_t pattern[PAT_LEN];
static uint8_t row = 0;
static uint8_t tick = 0;
static uint8_t tps = 8;

void pulse_music_init(void) {
    for (int i=0;i<PAT_LEN;i++) {
        pattern[i].note = 0;
        pattern[i].length = 0;
        pattern[i].vel = 0;
    }
    pattern[0] = (step_t){60,4,100};
    pattern[4] = (step_t){64,4,100};
    pattern[8] = (step_t){67,4,100};
    pattern[12]= (step_t){72,4,100};
}

void pulse_music_update(void) {
    if (++tick < tps) return;
    tick = 0;

    step_t *st = &pattern[row];
    if (st->length && st->vel) {
        synth_note_on(st->note, st->vel);
        drums_trigger(0, 80.0f, 0.02f, 800);
    }

    row++;
    if (row >= PAT_LEN) row = 0;
}

void pulse_music_cmd(const char *s) {
    if (strncmp(s, "STEP=", 5) != 0) return;
    int step = atoi(s + 5);
    const char *n = strstr(s, "NOTE=");
    const char *l = strstr(s, "LEN=");
    const char *v = strstr(s, "VEL=");
    if (!n || !l || !v) return;
    if (step < 0 || step >= PAT_LEN) return;

    pattern[step].note   = atoi(n + 5);
    pattern[step].length = atoi(l + 4);
    pattern[step].vel    = atoi(v + 4);
}
