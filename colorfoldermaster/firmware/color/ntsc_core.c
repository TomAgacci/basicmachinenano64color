#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "ntsc_core.h"
#include "framebuffer.h"
#include "palettes.h"

// -------------------- NTSC constants --------------------

#define YR  77
#define YG 150
#define YB  29

#define IR 152
#define IG -70
#define IB -82

#define QR  54
#define QG -134
#define QB  79

// PHASE_INC ≈ (3.579545 MHz / ~8 MHz sample) * 2^32
#define PHASE_INC 0x6BCA1AF3UL

// Ramanujan line phase: frac(pi*sqrt(163)) * 2^16 ≈ 0xC3A5
#define RAMANUJAN_INC 0xC3A5

// -------------------- Global state --------------------

volatile uint16_t ntsc_line = 0;
volatile uint8_t ntsc_render_flag = 0;

volatile uint32_t ntsc_phase = 0;
volatile uint16_t ntsc_line_phase = 0;

volatile uint8_t ntsc_palette_bank = 0;

// -------------------- Framebuffer --------------------

uint8_t ntsc_framebuffer[NTSC_HEIGHT][NTSC_WIDTH];

// -------------------- Quarter‑wave cosine LUT --------------------

static const uint8_t cos_qw[64] PROGMEM = {
  255,254,254,253,252,250,248,245,
  242,239,235,231,226,221,216,210,
  204,198,191,184,177,170,162,154,
  146,138,130,121,113,104,95, 86,
  77, 68, 59, 50, 41, 33, 25, 17,
   9,  2,  0,  0,  0,  2,  9, 17,
  25, 33, 41, 50, 59, 68, 77, 86,
  95,104,113,121,130,138,146,154
};

// -------------------- Fast trig --------------------

static inline uint8_t fast_cos(uint8_t ph) {
    uint8_t q = ph & 0x3F;
    uint8_t s = ph >> 6;

    switch (s) {
        case 0: return pgm_read_byte(&cos_qw[q]);
        case 1: return pgm_read_byte(&cos_qw[63 - q]);
        case 2: return 255 - pgm_read_byte(&cos_qw[q]);
        default:return 255 - pgm_read_byte(&cos_qw[63 - q]);
    }
}

static inline uint8_t fast_sin(uint8_t ph) {
    return fast_cos(ph + 64);
}

// -------------------- RGB → YIQ (banked) --------------------

static inline void rgb_to_yiq(uint8_t pix, int16_t *Y, int16_t *I, int16_t *Q) {
    uint8_t bank = ntsc_palette_bank;

    uint8_t r = pgm_read_byte(&palette_r[bank][pix]);
    uint8_t g = pgm_read_byte(&palette_g[bank][pix]);
    uint8_t b = pgm_read_byte(&palette_b[bank][pix]);

    *Y = (YR*r + YG*g + YB*b) >> 8;
    *I = (IR*r + IG*g + IB*b) >> 8;
    *Q = (QR*r + QG*g + QB*b) >> 8;
}

// -------------------- Timer1 ISR --------------------

ISR(TIMER1_COMPA_vect) {
    ntsc_render_flag = 1;

    if (++ntsc_line >= NTSC_LINES_PER_FRAME)
        ntsc_line = 0;

    ntsc_line_phase += RAMANUJAN_INC;
}

// -------------------- Public API --------------------

void ntsc_set_palette_bank(uint8_t bank) {
    ntsc_palette_bank = bank & (NTSC_NBANKS - 1);
}

void ntsc_init(void) {
    // PORTD as 8‑bit DAC
    DDRD = 0xFF;
    PORTD = NTSC_LEVEL_BLANK;

    // Timer1 CTC: one interrupt per scanline
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS10);
    OCR1A  = 1015;                 // 1016 cycles per line
    TIMSK1 = (1 << OCIE1A);

    sei();
}

void ntsc_render_frame_step(void) {
    if (!ntsc_render_flag) return;
    ntsc_render_flag = 0;

    uint16_t line = ntsc_line;
    uint16_t n = 0;

    // --- 1. H‑SYNC ---
    uint8_t c = 75;
    do {
        PORTD = NTSC_LEVEL_SYNC;
    } while (--c);

    // --- 2. BACK PORCH ---
    c = 93;
    do {
        PORTD = NTSC_LEVEL_BLANK;
    } while (--c);

    // --- 3. COLOR BURST ---
    uint32_t local_phase = ntsc_phase + ((uint32_t)ntsc_line_phase << 8);
    uint8_t i = 20;
    do {
        local_phase += PHASE_INC;
        uint8_t ph = (local_phase >> 24) + 128; // 180°
        uint8_t cval = fast_cos(ph);
        int16_t Vb = NTSC_LEVEL_BLANK + ((int8_t)(cval - 128) >> 2);
        PORTD = (uint8_t)Vb;
    } while (--i);

    // --- 4. ACTIVE VIDEO ---
    uint16_t cnt = 832;
    uint16_t x = 0;

    do {
        uint8_t pix = 0;
        if (line < NTSC_HEIGHT && x < NTSC_WIDTH) {
            pix = ntsc_framebuffer[line][x];
        }

        int16_t Y,I,Q;
        rgb_to_yiq(pix, &Y,&I,&Q);

        local_phase += PHASE_INC;
        uint8_t ph = local_phase >> 24;
        uint8_t ci = fast_cos(ph);
        uint8_t si = fast_sin(ph);

        int16_t Ci = (int8_t)(ci - 128);
        int16_t Si = (int8_t)(si - 128);

        int16_t Cc = ((I * Ci) + (Q * Si)) >> 8;

        int16_t V = NTSC_LEVEL_BLANK + ((Y * 179) >> 8) + Cc;

        if (V < 0)   V = 0;
        if (V > 255) V = 255;

        PORTD = (uint8_t)V;

        x++;
    } while (--cnt);

    ntsc_phase = local_phase;

    // --- 5. FRONT PORCH ---
    c = 16;
    do {
        PORTD = NTSC_LEVEL_BLANK;
    } while (--c);
}
