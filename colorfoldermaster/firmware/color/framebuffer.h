#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

#include "ntsc_core.h"

// Simple 256x192 8‑bit framebuffer.
// If this is too big for your build, you can replace it with a fetch_pixel() API.
extern uint8_t ntsc_framebuffer[NTSC_HEIGHT][NTSC_WIDTH];

#endif
