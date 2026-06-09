#pragma once
#include <Arduino.h>

// Composite output pin (D9 on Nano)
#define VIDEO_PIN 9

// Logical levels (mapped via resistor ladder / voltage)
enum VideoLevel : uint8_t {
  LEVEL_SYNC  = 0,
  LEVEL_BLACK = 1,
  LEVEL_WHITE = 2
};

struct Pulse {
  uint16_t ticks;
  uint8_t  level;
};

void pulseVideoInit();

// Called internally by ISR to build visible scanlines
void pulseVideoRenderScanline(uint16_t line);
