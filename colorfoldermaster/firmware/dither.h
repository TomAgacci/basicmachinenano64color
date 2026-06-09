#pragma once
#include <Arduino.h>

// Initialize dither patterns based on palette luma
void ditherInit();

// Return 0/1 for a 2x2 dither tile for a given color index.
// x,y are 0 or 1 (within the 2x2 cell).
uint8_t dither2x2(uint8_t colorIndex, uint8_t x, uint8_t y);
