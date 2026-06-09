#include "logical384.h"

LogicalPixel logical384Sample(uint16_t lx, uint16_t ly) {
  LogicalPixel p;
  p.color = 0;
  p.on    = false;

  if (lx >= LOGICAL_WIDTH || ly >= LOGICAL_HEIGHT) {
    return p;
  }

  // Map 384x240 → 32x24 text grid
  // Each text cell is 12x10 logical pixels
  uint8_t col = (uint32_t)lx * 32 / LOGICAL_WIDTH;   // 0..31
  uint8_t row = (uint32_t)ly * 24 / LOGICAL_HEIGHT;  // 0..23

  if (col >= 32 || row >= 24) {
    return p;
  }

  // Local coords inside the text cell
  uint16_t cellW = LOGICAL_WIDTH / 32;   // 12
  uint16_t cellH = LOGICAL_HEIGHT / 24;  // 10

  uint16_t localX = lx - col * cellW;    // 0..11
  uint16_t localY = ly - row * cellH;    // 0..9

  // Map 12x10 → 8x8 glyph space
  uint8_t glyphX = (uint32_t)localX * 8 / cellW; // 0..7
  uint8_t glyphY = (uint32_t)localY * 8 / cellH; // 0..7

  uint8_t ch = textGetChar(col, row);
  const uint8_t* glyph = fontGetGlyph(ch);
  uint8_t bits = glyph[glyphY];

  bool on = (bits & (0x80 >> glyphX)) != 0;
  uint8_t color = colorLayerGet(col, row);

  p.on = on;
  p.color = color & 0x0F;
  return p;
}
