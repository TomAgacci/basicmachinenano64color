#include "colorlayer.h"
#include "palette.h"
#include "textmode.h"

static uint8_t g_colorBuf[32 * 24]; // matches text buffer size

void colorLayerInit() {
  for (uint16_t i = 0; i < sizeof(g_colorBuf); ++i) {
    g_colorBuf[i] = 15; // default: brightest color
  }
}

void colorLayerSet(uint8_t x, uint8_t y, uint8_t colorIndex) {
  if (x >= 32 || y >= 24) return;
  g_colorBuf[y * 32 + x] = (colorIndex & 0x0F);
}

uint8_t colorLayerGet(uint8_t x, uint8_t y) {
  if (x >= 32 || y >= 24) return 0;
  return g_colorBuf[y * 32 + x] & 0x0F;
}

uint8_t colorLayerIndexForGrey(uint8_t grey) {
  uint8_t bestIndex = 0;
  uint16_t bestDiff = 0xFFFF;

  for (uint8_t i = 0; i < 16; ++i) {
    uint8_t lum = paletteGetLuma(i);
    int16_t diff = (int16_t)lum - (int16_t)grey;
    if (diff < 0) diff = -diff;
    if ((uint16_t)diff < bestDiff) {
      bestDiff = (uint16_t)diff;
      bestIndex = i;
    }
  }
  return bestIndex;
}
