#include "palette.h"
#include <math.h>

static ColorRGB g_palette[16];
static uint8_t g_luma[16];

static float clamp01(float x) {
  if (x < 0.0f) return 0.0f;
  if (x > 1.0f) return 1.0f;
  return x;
}

void paletteInit() {
  // Ramanujan-flavored seed:
  // y = (pi * sqrt(163))^(1 / (pi * sqrt(163)))
  const float s = PI * sqrt(163.0f);
  const float seed = powf(s, 1.0f / s);

  for (int i = 0; i < 16; ++i) {
    float t = (i + 1) * seed;

    float r = clamp01(sinf(t * 3.1f) * 0.5f + 0.5f);
    float g = clamp01(sinf(t * 5.7f) * 0.5f + 0.5f);
    float b = clamp01(sinf(t * 7.3f) * 0.5f + 0.5f);

    uint8_t R = (uint8_t)(r * 255.0f + 0.5f);
    uint8_t G = (uint8_t)(g * 255.0f + 0.5f);
    uint8_t B = (uint8_t)(b * 255.0f + 0.5f);

    g_palette[i].r = R;
    g_palette[i].g = G;
    g_palette[i].b = B;

    uint16_t lum = (uint16_t)(0.299f * R + 0.587f * G + 0.114f * B + 0.5f);
    if (lum > 255) lum = 255;
    g_luma[i] = (uint8_t)lum;
  }
}

const ColorRGB& paletteGet(uint8_t index) {
  index &= 0x0F;
  return g_palette[index];
}

uint8_t paletteGetLuma(uint8_t index) {
  index &= 0x0F;
  return g_luma[index];
}
