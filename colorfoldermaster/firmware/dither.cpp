#include "dither.h"
#include "palette.h"

// For each color, store a 2x2 tile as 4 bits:
// bit 0: (0,0), bit 1: (1,0), bit 2: (0,1), bit 3: (1,1)
static uint8_t g_ditherTile[16];

void ditherInit() {
  for (uint8_t i = 0; i < 16; ++i) {
    uint8_t lum = paletteGetLuma(i); // 0–255
    // Map luma to 0–4 "on" pixels in the 2x2 tile
    uint8_t count = (uint8_t)((lum * 5UL) / 256UL); // 0..4

    uint8_t mask = 0;
    if (count > 0) mask |= (1 << 0); // (0,0)
    if (count > 1) mask |= (1 << 1); // (1,0)
    if (count > 2) mask |= (1 << 2); // (0,1)
    if (count > 3) mask |= (1 << 3); // (1,1)

    g_ditherTile[i] = mask;
  }
}

uint8_t dither2x2(uint8_t colorIndex, uint8_t x, uint8_t y) {
  colorIndex &= 0x0F;
  x &= 0x01;
  y &= 0x01;

  uint8_t bitIndex = (y << 1) | x; // 0..3
  uint8_t mask = g_ditherTile[colorIndex];
  return (mask >> bitIndex) & 0x01;
}
