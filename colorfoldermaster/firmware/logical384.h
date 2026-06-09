#pragma once
#include <Arduino.h>
#include "colorlayer.h"
#include "textmode.h"
#include "font.h"

// Logical resolution
#define LOGICAL_WIDTH  384
#define LOGICAL_HEIGHT 240

struct LogicalPixel {
  uint8_t color; // 0–15
  bool    on;    // glyph bit
};

// Sample the logical 384x240 space and return a pixel
LogicalPixel logical384Sample(uint16_t lx, uint16_t ly);
