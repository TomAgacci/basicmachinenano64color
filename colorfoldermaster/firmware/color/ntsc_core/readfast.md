🧩 2. Convert to 4 × 256‑entry Nano64 palette banks
Each bank repeats the 64‑entry palette 4 times:

c
// palettes.c
#include <avr/pgmspace.h>
#include "palettes.h"
#include "c64_palette.h"

#define REPEAT64_R  C64_R[0],C64_R[1],C64_R[2],C64_R[3],C64_R[4],C64_R[5],C64_R[6],C64_R[7], \
                    C64_R[8],C64_R[9],C64_R[10],C64_R[11],C64_R[12],C64_R[13],C64_R[14],C64_R[15], \
                    C64_R[16],C64_R[17],C64_R[18],C64_R[19],C64_R[20],C64_R[21],C64_R[22],C64_R[23], \
                    C64_R[24],C64_R[25],C64_R[26],C64_R[27],C64_R[28],C64_R[29],C64_R[30],C64_R[31], \
                    C64_R[32],C64_R[33],C64_R[34],C64_R[35],C64_R[36],C64_R[37],C64_R[38],C64_R[39], \
                    C64_R[40],C64_R[41],C64_R[42],C64_R[43],C64_R[44],C64_R[45],C64_R[46],C64_R[47], \
                    C64_R[48],C64_R[49],C64_R[50],C64_R[51],C64_R[52],C64_R[53],C64_R[54],C64_R[55], \
                    C64_R[56],C64_R[57],C64_R[58],C64_R[59],C64_R[60],C64_R[61],C64_R[62],C64_R[63]

#define REPEAT64_G  /* same pattern using C64_G */
#define REPEAT64_B  /* same pattern using C64_B */

const uint8_t palette_r[4][256] PROGMEM = {
  { REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R },
  { REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R },
  { REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R },
  { REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R }
};

const uint8_t palette_g[4][256] PROGMEM = {
  { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G },
  { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G },
  { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G },
  { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G }
};

const uint8_t palette_b[4][256] PROGMEM = {
  { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B },
  { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B },
  { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B },
  { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B }
};
This is now Nano64‑ready.
