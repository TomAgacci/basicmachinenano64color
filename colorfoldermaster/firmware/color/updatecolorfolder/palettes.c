#include "palettes.h"
#include "../colorfoldermaster/colorfoldermaster.h"

// Expand Nano64's 64-color palette into 256 entries.
// Each bank is 256 entries, but we repeat the 64-color palette 4 times.

const uint8_t palette_r[NTSC_NBANKS][256] PROGMEM = {
    {   // Bank 0 (Nano64 default)
        #define REPEAT64_R \
            color_r[0], color_r[1], color_r[2], color_r[3], \
            color_r[4], color_r[5], color_r[6], color_r[7], \
            color_r[8], color_r[9], color_r[10], color_r[11], \
            color_r[12], color_r[13], color_r[14], color_r[15], \
            color_r[16], color_r[17], color_r[18], color_r[19], \
            color_r[20], color_r[21], color_r[22], color_r[23], \
            color_r[24], color_r[25], color_r[26], color_r[27], \
            color_r[28], color_r[29], color_r[30], color_r[31], \
            color_r[32], color_r[33], color_r[34], color_r[35], \
            color_r[36], color_r[37], color_r[38], color_r[39], \
            color_r[40], color_r[41], color_r[42], color_r[43], \
            color_r[44], color_r[45], color_r[46], color_r[47], \
            color_r[48], color_r[49], color_r[50], color_r[51], \
            color_r[52], color_r[53], color_r[54], color_r[55], \
            color_r[56], color_r[57], color_r[58], color_r[59], \
            color_r[60], color_r[61], color_r[62], color_r[63]

        REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R
    },

    {   // Bank 1 (duplicate for now)
        REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R
    },

    {   // Bank 2
        REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R
    },

    {   // Bank 3
        REPEAT64_R, REPEAT64_R, REPEAT64_R, REPEAT64_R
    }
};

const uint8_t palette_g[NTSC_NBANKS][256] PROGMEM = {
    {   // Bank 0
        #define REPEAT64_G \
            color_g[0], color_g[1], color_g[2], color_g[3], \
            color_g[4], color_g[5], color_g[6], color_g[7], \
            color_g[8], color_g[9], color_g[10], color_g[11], \
            color_g[12], color_g[13], color_g[14], color_g[15], \
            color_g[16], color_g[17], color_g[18], color_g[19], \
            color_g[20], color_g[21], color_g[22], color_g[23], \
            color_g[24], color_g[25], color_g[26], color_g[27], \
            color_g[28], color_g[29], color_g[30], color_g[31], \
            color_g[32], color_g[33], color_g[34], color_g[35], \
            color_g[36], color_g[37], color_g[38], color_g[39], \
            color_g[40], color_g[41], color_g[42], color_g[43], \
            color_g[44], color_g[45], color_g[46], color_g[47], \
            color_g[48], color_g[49], color_g[50], color_g[51], \
            color_g[52], color_g[53], color_g[54], color_g[55], \
            color_g[56], color_g[57], color_g[58], color_g[59], \
            color_g[60], color_g[61], color_g[62], color_g[63]

        REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G
    },

    { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G },
    { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G },
    { REPEAT64_G, REPEAT64_G, REPEAT64_G, REPEAT64_G }
};

const uint8_t palette_b[NTSC_NBANKS][256] PROGMEM = {
    {   // Bank 0
        #define REPEAT64_B \
            color_b[0], color_b[1], color_b[2], color_b[3], \
            color_b[4], color_b[5], color_b[6], color_b[7], \
            color_b[8], color_b[9], color_b[10], color_b[11], \
            color_b[12], color_b[13], color_b[14], color_b[15], \
            color_b[16], color_b[17], color_b[18], color_b[19], \
            color_b[20], color_b[21], color_b[22], color_b[23], \
            color_b[24], color_b[25], color_b[26], color_b[27], \
            color_b[28], color_b[29], color_b[30], color_b[31], \
            color_b[32], color_b[33], color_b[34], color_b[35], \
            color_b[36], color_b[37], color_b[38], color_b[39], \
            color_b[40], color_b[41], color_b[42], color_b[43], \
            color_b[44], color_b[45], color_b[46], color_b[47], \
            color_b[48], color_b[49], color_b[50], color_b[51], \
            color_b[52], color_b[53], color_b[54], color_b[55], \
            color_b[56], color_b[57], color_b[58], color_b[59], \
            color_b[60], color_b[61], color_b[62], color_b[63]

        REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B
    },

    { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B },
    { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B },
    { REPEAT64_B, REPEAT64_B, REPEAT64_B, REPEAT64_B }
};
