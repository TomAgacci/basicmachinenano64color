#include <Arduino.h>
#include "pulse_video.h"
#include "textmode.h"
#include "sprites.h"
#include "palette.h"
#include "dither.h"
#include "colorlayer.h"

// Simple banner text
static const char* BOOT_LINES[] = {
  "NANO64-COMPOSITE",
  "MONOCHROME + RAMANUJAN COLOR",
  "RES: 256x192 (32x24 TEXT)",
  "16 LOGICAL COLORS (DITHERED)",
  "",
  "READY."
};

void setup() {
  // Video pin + timer
  pulseVideoInit();

  // Core subsystems
  textInit();
  spritesInit();
  paletteInit();
  ditherInit();
  colorLayerInit();

  // Boot screen
  textClear();
  for (uint8_t i = 0; i < 6; ++i) {
    textPrintAt(2, 2 + i * 2, BOOT_LINES[i]);
  }

  // Give the banner some color indices
  for (uint8_t i = 0; i < 6; ++i) {
    uint8_t len = strlen(BOOT_LINES[i]);
    for (uint8_t x = 0; x < len; ++x) {
      // Cycle colors 1..15
      colorLayerSet(2 + x, 2 + i * 2, 1 + ((x + i) & 0x0F));
    }
  }
}

void loop() {
  // Placeholder main loop:
  // - could run BASIC
  // - could animate sprites
  // - keep it light so video ISR can run

  static uint32_t t = 0;
  t++;

  // Simple demo: animate a color bar on the last line
  if ((t & 7) == 0) {
    uint8_t y = 23;
    for (uint8_t x = 0; x < 32; ++x) {
      textSetChar(x, y, ' ');
      colorLayerSet(x, y, (x + (t >> 5)) & 0x0F);
    }
  }
}
