#include "pulse_video.h"
#include "textmode.h"
#include "colorlayer.h"
#include "dither.h"
#include "font.h"

// Rough NTSC-like timing (not broadcast-accurate, but structurally OK)
static const uint16_t HSYNC_TICKS   = 75;   // ~4.7us @16MHz
static const uint16_t BACKPORCH_TKS = 75;   // ~4.7us
static const uint16_t ACTIVE_TICKS  = 800;  // active region (approx)

// Frame parameters
static const uint16_t LINES_TOTAL   = 262;
static const uint16_t LINES_VISIBLE = 192;
static const uint16_t FIRST_VISIBLE = 40;   // vertical offset

// Line pulse buffer
static Pulse lineBuf[1024];
static volatile uint16_t lineLen = 0;
static volatile uint16_t linePos = 0;
static volatile uint16_t pulseTicksLeft = 0;

// Current scanline
static volatile uint16_t currentLine = 0;

ISR(TIMER1_COMPA_vect) {
  if (pulseTicksLeft == 0) {
    // End of line?
    if (linePos >= lineLen) {
      // Next line
      currentLine++;
      if (currentLine >= LINES_TOTAL) {
        currentLine = 0;
      }

      // Build pulses for new line
      // HSYNC
      lineBuf[0] = { HSYNC_TICKS, LEVEL_SYNC };
      // Back porch
      lineBuf[1] = { BACKPORCH_TKS, LEVEL_BLACK };
      lineLen = 2;

      if (currentLine >= FIRST_VISIBLE &&
          currentLine < FIRST_VISIBLE + LINES_VISIBLE) {
        uint16_t visibleLine = currentLine - FIRST_VISIBLE;
        pulseVideoRenderScanline(visibleLine);
      }

      linePos = 0;
    }

    Pulse p = lineBuf[linePos++];
    pulseTicksLeft = p.ticks;

    // Map level to pin state (simple 2-level for now)
    if (p.level == LEVEL_SYNC) {
      digitalWrite(VIDEO_PIN, LOW);
    } else {
      digitalWrite(VIDEO_PIN, HIGH);
    }
  }

  pulseTicksLeft--;
}

void pulseVideoInit() {
  pinMode(VIDEO_PIN, OUTPUT);
  digitalWrite(VIDEO_PIN, LOW);

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);    // prescaler 1 (16MHz)
  OCR1A = 1;                // 1 tick per compare
  TIMSK1 |= (1 << OCIE1A);  // enable compare interrupt
  sei();

  currentLine = 0;
  linePos = lineLen = 0;
  pulseTicksLeft = 0;
}

// Render one visible scanline (0..191) into lineBuf
void pulseVideoRenderScanline(uint16_t line) {
  // Text grid: 32x24, each tile 8x8
  const uint8_t tileHeight = 8;
  uint8_t row = line / tileHeight;       // 0..23
  uint8_t glyphRow = line % tileHeight;  // 0..7

  const uint8_t cols = 32;

  uint16_t idx = lineLen; // start after sync + back porch

  // For each text column, fetch char + color, then glyph row, then dither
  for (uint8_t col = 0; col < cols; ++col) {
    uint8_t ch = textGetChar(col, row);
    uint8_t colorIndex = colorLayerGet(col, row);

    const uint8_t* glyph = fontGetGlyph(ch);
    uint8_t bits = glyph[glyphRow];

    // 8 pixels per glyph
    for (uint8_t b = 0; b < 8; ++b) {
      bool on = bits & (0x80 >> b);

      // Apply 2x2 dither based on color index and subpixel position
      // Map line + column pixel to 2x2 cell
      uint8_t pixelX = col * 8 + b;
      uint8_t pixelY = line;

      uint8_t dx = pixelX & 0x01;
      uint8_t dy = pixelY & 0x01;

      uint8_t d = dither2x2(colorIndex, dx, dy);

      bool finalOn = on && (d != 0);

      lineBuf[idx++] = { 1, finalOn ? LEVEL_WHITE : LEVEL_BLACK };
    }
  }

  lineLen = idx;
}
