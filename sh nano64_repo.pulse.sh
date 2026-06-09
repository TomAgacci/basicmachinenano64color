#!/usr/bin/env sh
set -e

ROOT="nano64"

echo "Rebuilding Nano64 repo into ./$ROOT"
mkdir -p "$ROOT/firmware" "$ROOT/web" "$ROOT/tools" "$ROOT/assets"

###############################################################################
# README.md
###############################################################################
cat > "$ROOT/README.md" << 'EOF'
# Nano64 – Basic Machine

Nano64 is a tiny “fantasy computer” inspired by 8‑bit micros and fantasy consoles.
This repo contains:

- `firmware/` – core CPU, video, textmode, sprites, and font for the Nano64 machine
- `web/` – browser emulator for Nano64 video + textmode
- `assets/` – default font, palette, and sprite data (in simple text form here)
- `tools/` – helper scripts for packing assets (placeholder stubs)

This is a *basic machine* build: 64×32 text, 256×192 pixel logical space, 16‑color palette.
EOF

###############################################################################
# firmware/nano64_firmware.ino
###############################################################################
cat > "$ROOT/firmware/nano64_firmware.ino" << 'EOF'
#include "cpu_core.h"
#include "video_color.h"
#include "textmode.h"
#include "sprites.h"
#include "font.h"

NanoCPU cpu;
VideoColor video;
TextMode textmode(video);
SpriteSystem sprites(video);

void setup() {
  video.begin();
  textmode.begin();
  sprites.begin();

  textmode.clear();
  textmode.print(0, 0, "NANO64 BASIC MACHINE");
  textmode.print(0, 2, "HELLO, WORLD.");
  textmode.print(0, 4, "SPRITE DEMO RUNNING...");
}

uint8_t t = 0;

void loop() {
  t++;

  if (t % 8 == 0) {
    textmode.scrollUp();
    textmode.print(0, 31, "TICK: ");
    textmode.printNumber(6, 31, t);
  }

  float angle = (t % 256) * 0.0245436926f; // 2π / 256
  int cx = 80 + (int)(40 * cos(angle));
  int cy = 60 + (int)(30 * sin(angle));

  sprites.clear();
  sprites.drawSprite(0, cx, cy, 1);

  video.beginFrame();
  textmode.render();
  sprites.render();
  video.endFrame();
}
EOF

###############################################################################
# firmware/cpu_core.h
###############################################################################
cat > "$ROOT/firmware/cpu_core.h" << 'EOF'
#pragma once
#include <stdint.h>

struct NanoCPU {
  uint8_t  A, X, Y;
  uint16_t PC;
  uint8_t  SP;
  uint8_t  flags;

  void reset() {
    A = X = Y = 0;
    PC = 0x0000;
    SP = 0xFF;
    flags = 0;
  }

  void step() {
    // Placeholder: no real instruction set yet.
  }
};
EOF

###############################################################################
# firmware/cpu_core.cpp
###############################################################################
cat > "$ROOT/firmware/cpu_core.cpp" << 'EOF'
#include "cpu_core.h"
// Instruction decoding will live here later.
EOF

###############################################################################
# firmware/video_color.h
###############################################################################
cat > "$ROOT/firmware/video_color.h" << 'EOF'
#pragma once
#include <stdint.h>

static const int NANO64_WIDTH  = 256;
static const int NANO64_HEIGHT = 192;
static const int NANO64_COLORS = 16;

struct RGB {
  uint8_t r, g, b;
};

class VideoColor {
public:
  uint8_t framebuffer[NANO64_WIDTH * NANO64_HEIGHT];
  RGB     palette[NANO64_COLORS];

  void begin();
  void beginFrame();
  void endFrame();

  void clear(uint8_t colorIndex = 0);
  void putPixel(int x, int y, uint8_t colorIndex);
};
EOF

###############################################################################
# firmware/video_color.cpp
###############################################################################
cat > "$ROOT/firmware/video_color.cpp" << 'EOF'
#include "video_color.h"

void VideoColor::begin() {
  palette[0]  = { 0x00, 0x00, 0x00 };
  palette[1]  = { 0x1D, 0x2B, 0x53 };
  palette[2]  = { 0x7E, 0x25, 0x53 };
  palette[3]  = { 0x00, 0x87, 0x51 };
  palette[4]  = { 0xAB, 0x52, 0x36 };
  palette[5]  = { 0x5F, 0x57, 0x4F };
  palette[6]  = { 0xC2, 0xC3, 0xC7 };
  palette[7]  = { 0xFF, 0xF1, 0xE8 };
  palette[8]  = { 0xFF, 0x00, 0x4D };
  palette[9]  = { 0xFF, 0xA3, 0x00 };
  palette[10] = { 0xFF, 0xEC, 0x27 };
  palette[11] = { 0x00, 0xE4, 0x36 };
  palette[12] = { 0x29, 0xAD, 0xFF };
  palette[13] = { 0x83, 0x76, 0x9C };
  palette[14] = { 0xFF, 0x77, 0xA8 };
  palette[15] = { 0xFF, 0xCC, 0xAA };

  clear(0);
}

void VideoColor::beginFrame() {
}

void VideoColor::endFrame() {
}

void VideoColor::clear(uint8_t colorIndex) {
  for (int i = 0; i < NANO64_WIDTH * NANO64_HEIGHT; ++i) {
    framebuffer[i] = colorIndex;
  }
}

void VideoColor::putPixel(int x, int y, uint8_t colorIndex) {
  if (x < 0 || y < 0 || x >= NANO64_WIDTH || y >= NANO64_HEIGHT) return;
  framebuffer[y * NANO64_WIDTH + x] = colorIndex;
}
EOF

###############################################################################
# firmware/textmode.h
###############################################################################
cat > "$ROOT/firmware/textmode.h" << 'EOF'
#pragma once
#include <stdint.h>
#include "video_color.h"
#include "font.h"

static const int NANO64_COLS = 32;
static const int NANO64_ROWS = 24;

class TextMode {
public:
  TextMode(VideoColor &v) : video(v) {}

  void begin();
  void clear(uint8_t colorIndex = 7);
  void print(int col, int row, const char *text, uint8_t colorIndex = 7);
  void printNumber(int col, int row, int value, uint8_t colorIndex = 7);
  void scrollUp();
  void render();

private:
  VideoColor &video;
  uint8_t chars[NANO64_COLS * NANO64_ROWS];
  uint8_t colors[NANO64_COLS * NANO64_ROWS];
};
EOF

###############################################################################
# firmware/textmode.cpp
###############################################################################
cat > "$ROOT/firmware/textmode.cpp" << 'EOF'
#include "textmode.h"
#include <string.h>
#include <stdio.h>

void TextMode::begin() {
  clear();
}

void TextMode::clear(uint8_t colorIndex) {
  for (int i = 0; i < NANO64_COLS * NANO64_ROWS; ++i) {
    chars[i]  = ' ';
    colors[i] = colorIndex;
  }
}

void TextMode::print(int col, int row, const char *text, uint8_t colorIndex) {
  if (row < 0 || row >= NANO64_ROWS) return;
  int idx = row * NANO64_COLS + col;
  while (*text && idx < NANO64_COLS * NANO64_ROWS) {
    chars[idx]  = (uint8_t)*text++;
    colors[idx] = colorIndex;
    idx++;
  }
}

void TextMode::printNumber(int col, int row, int value, uint8_t colorIndex) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", value);
  print(col, row, buf, colorIndex);
}

void TextMode::scrollUp() {
  memmove(chars,  chars + NANO64_COLS,  (NANO64_ROWS - 1) * NANO64_COLS);
  memmove(colors, colors + NANO64_COLS, (NANO64_ROWS - 1) * NANO64_COLS);
  for (int c = 0; c < NANO64_COLS; ++c) {
    chars[(NANO64_ROWS - 1) * NANO64_COLS + c]  = ' ';
    colors[(NANO64_ROWS - 1) * NANO64_COLS + c] = 7;
  }
}

void TextMode::render() {
  for (int row = 0; row < NANO64_ROWS; ++row) {
    for (int col = 0; col < NANO64_COLS; ++col) {
      int idx = row * NANO64_COLS + col;
      uint8_t ch = chars[idx];
      uint8_t colorIndex = colors[idx];
      drawGlyph(video, col * 8, row * 8, ch, colorIndex);
    }
  }
}
EOF

###############################################################################
# firmware/sprites.h
###############################################################################
cat > "$ROOT/firmware/sprites.h" << 'EOF'
#pragma once
#include <stdint.h>
#include "video_color.h"

static const int NANO64_MAX_SPRITES = 32;

struct Sprite {
  int16_t x, y;
  uint8_t index;
  uint8_t color;
  bool    active;
};

class SpriteSystem {
public:
  SpriteSystem(VideoColor &v) : video(v) {}

  void begin();
  void clear();
  void drawSprite(int slot, int x, int y, uint8_t colorIndex);
  void render();

private:
  VideoColor &video;
  Sprite sprites[NANO64_MAX_SPRITES];
};
EOF

###############################################################################
# firmware/sprites.cpp
###############################################################################
cat > "$ROOT/firmware/sprites.cpp" << 'EOF'
#include "sprites.h"
#include "font.h"

void SpriteSystem::begin() {
  clear();
}

void SpriteSystem::clear() {
  for (int i = 0; i < NANO64_MAX_SPRITES; ++i) {
    sprites[i].active = false;
  }
}

void SpriteSystem::drawSprite(int slot, int x, int y, uint8_t colorIndex) {
  if (slot < 0 || slot >= NANO64_MAX_SPRITES) return;
  sprites[slot].x      = x;
  sprites[slot].y      = y;
  sprites[slot].index  = 1;
  sprites[slot].color  = colorIndex;
  sprites[slot].active = true;
}

void SpriteSystem::render() {
  for (int i = 0; i < NANO64_MAX_SPRITES; ++i) {
    if (!sprites[i].active) continue;
    drawGlyph(video, sprites[i].x, sprites[i].y, sprites[i].index, sprites[i].color);
  }
}
EOF

###############################################################################
# firmware/font.h
###############################################################################
cat > "$ROOT/firmware/font.h" << 'EOF'
#pragma once
#include <stdint.h>
#include "video_color.h"

extern const uint8_t NANO64_FONT[128][8];

void drawGlyph(VideoColor &video, int x, int y, uint8_t ch, uint8_t colorIndex);
EOF

###############################################################################
# firmware/font.cpp
###############################################################################
cat > "$ROOT/firmware/font.cpp" << 'EOF'
#include "font.h"

const uint8_t NANO64_FONT[128][8] = {
  {0,0,0,0,0,0,0,0}, // 0
};

static const uint8_t GLYPH_SPACE[8] = {0,0,0,0,0,0,0,0};
static const uint8_t GLYPH_A[8] = {
  0b00111100,
  0b01000010,
  0b01000010,
  0b01111110,
  0b01000010,
  0b01000010,
  0b01000010,
  0b00000000
};

static const uint8_t GLYPH_EXCL[8] = {
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00000000,
  0b00011000,
  0b00000000
};

void drawGlyph(VideoColor &video, int x, int y, uint8_t ch, uint8_t colorIndex) {
  const uint8_t *glyph = nullptr;

  if (ch == ' ') {
    glyph = GLYPH_SPACE;
  } else if (ch == 'A') {
    glyph = GLYPH_A;
  } else if (ch == '!') {
    glyph = GLYPH_EXCL;
  } else {
    glyph = GLYPH_SPACE;
  }

  for (int row = 0; row < 8; ++row) {
    uint8_t bits = glyph[row];
    for (int col = 0; col < 8; ++col) {
      if (bits & (0x80 >> col)) {
        video.putPixel(x + col, y + row, colorIndex);
      }
    }
  }
}
EOF

###############################################################################
# tools/pulse_compress.py
###############################################################################
cat > "$ROOT/tools/pulse_compress.py" << 'EOF'
#!/usr/bin/env python3
"""
pulse_compress.py – placeholder for future .pulse asset packing.
For now this just copies input to output.
"""
import sys
from pathlib import Path

def main():
  if len(sys.argv) != 3:
    print("Usage: pulse_compress.py <in> <out>", file=sys.stderr)
    sys.exit(1)
  src = Path(sys.argv[1]).read_bytes()
  Path(sys.argv[2]).write_bytes(src)

if __name__ == "__main__":
  main()
EOF
chmod +x "$ROOT/tools/pulse_compress.py"

###############################################################################
# tools/pulse_extract.py
###############################################################################
cat > "$ROOT/tools/pulse_extract.py" << 'EOF'
#!/usr/bin/env python3
"""
pulse_extract.py – placeholder for future .pulse asset extraction.
"""
import sys
from pathlib import Path

def main():
  if len(sys.argv) != 3:
    print("Usage: pulse_extract.py <in> <out>", file=sys.stderr)
    sys.exit(1)
  src = Path(sys.argv[1]).read_bytes()
  Path(sys.argv[2]).write_bytes(src)

if __name__ == "__main__":
  main()
EOF
chmod +x "$ROOT/tools/pulse_extract.py"

###############################################################################
# web/index.html
###############################################################################
cat > "$ROOT/web/index.html" << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Nano64 – Web Emulator</title>
  <style>
    body {
      margin: 0;
      background: #020617;
      color: #e5e7eb;
      font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
    }
    canvas {
      image-rendering: pixelated;
      border-radius: 8px;
      box-shadow: 0 18px 45px rgba(0, 0, 0, 0.7);
      background: #000;
    }
    .title {
      margin-bottom: 12px;
      font-size: 1rem;
      letter-spacing: 0.08em;
      text-transform: uppercase;
      color: #9ca3af;
    }
  </style>
</head>
<body>
  <div class="title">Nano64 – Basic Machine (Web)</div>
  <canvas id="screen" width="256" height="192"></canvas>
  <script src="main.js"></script>
</body>
</html>
EOF

###############################################################################
# web/main.js
###############################################################################
cat > "$ROOT/web/main.js" << 'EOF'
const WIDTH = 256;
const HEIGHT = 192;

const palette = [
  [0x00, 0x00, 0x00],
  [0x1D, 0x2B, 0x53],
  [0x7E, 0x25, 0x53],
  [0x00, 0x87, 0x51],
  [0xAB, 0x52, 0x36],
  [0x5F, 0x57, 0x4F],
  [0xC2, 0xC3, 0xC7],
  [0xFF, 0xF1, 0xE8],
  [0xFF, 0x00, 0x4D],
  [0xFF, 0xA3, 0x00],
  [0xFF, 0xEC, 0x27],
  [0x00, 0xE4, 0x36],
  [0x29, 0xAD, 0xFF],
  [0x83, 0x76, 0x9C],
  [0xFF, 0x77, 0xA8],
  [0xFF, 0xCC, 0xAA],
];

const canvas = document.getElementById("screen");
const ctx = canvas.getContext("2d");
const imageData = ctx.createImageData(WIDTH, HEIGHT);
const fb = new Uint8Array(WIDTH * HEIGHT);

function clear(colorIndex = 0) {
  fb.fill(colorIndex);
}

function putPixel(x, y, colorIndex) {
  if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) return;
  fb[y * WIDTH + x] = colorIndex;
}

let t = 0;

function drawFrame() {
  t++;

  clear(0);

  const cx = 128 + Math.round(60 * Math.cos(t * 0.05));
  const cy = 96 + Math.round(40 * Math.sin(t * 0.05));
  putPixel(cx, cy, 10);

  const msg = "NANO64 BASIC MACHINE";
  for (let i = 0; i < msg.length; i++) {
    const x = 8 + i * 8;
    const y = 8;
    drawChar(x, y, msg[i], 7);
  }

  blit();
  requestAnimationFrame(drawFrame);
}

const GLYPH_SPACE = [0,0,0,0,0,0,0,0];
const GLYPH_A = [
  0b00111100,
  0b01000010,
  0b01000010,
  0b01111110,
  0b01000010,
  0b01000010,
  0b01000010,
  0b00000000,
];

function glyphFor(ch) {
  if (ch === "A") return GLYPH_A;
  if (ch === " ") return GLYPH_SPACE;
  return GLYPH_SPACE;
}

function drawChar(x, y, ch, colorIndex) {
  const g = glyphFor(ch);
  for (let row = 0; row < 8; row++) {
    const bits = g[row];
    for (let col = 0; col < 8; col++) {
      if (bits & (0x80 >> col)) {
        putPixel(x + col, y + row, colorIndex);
      }
    }
  }
}

function blit() {
  let p = 0;
  for (let i = 0; i < fb.length; i++) {
    const [r, g, b] = palette[fb[i]];
    imageData.data[p++] = r;
    imageData.data[p++] = g;
    imageData.data[p++] = b;
    imageData.data[p++] = 255;
  }
  ctx.putImageData(imageData, 0, 0);
}

clear(0);
drawFrame();
EOF

###############################################################################
# assets/default_font.txt
###############################################################################
cat > "$ROOT/assets/default_font.txt" << 'EOF'
# Placeholder font asset for Nano64.
# In a future version this will be a packed .pulse file.
# For now, the firmware uses hardcoded glyphs.
EOF

echo "Done. Repo created at ./$ROOT"
