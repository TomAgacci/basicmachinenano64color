#pragma once
#include <Arduino.h>

struct ColorRGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

void paletteInit();
const ColorRGB& paletteGet(uint8_t index);
uint8_t paletteGetLuma(uint8_t index); // 0–255
