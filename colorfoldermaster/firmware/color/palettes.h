#ifndef PALETTES_H
#define PALETTES_H

#include <stdint.h>
#include <avr/pgmspace.h>

#include "ntsc_core.h"

// 4 banks × 256 colors each.
// Fill these with your Nano64 colorfoldermaster palette data.

extern const uint8_t palette_r[NTSC_NBANKS][256] PROGMEM;
extern const uint8_t palette_g[NTSC_NBANKS][256] PROGMEM;
extern const uint8_t palette_b[NTSC_NBANKS][256] PROGMEM;

#endif
