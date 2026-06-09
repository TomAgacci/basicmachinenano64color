#ifndef NTSC_CORE_H
#define NTSC_CORE_H

#include <stdint.h>

#define NTSC_LINES_PER_FRAME 262
#define NTSC_WIDTH           256
#define NTSC_HEIGHT          192
#define NTSC_NBANKS          4

// Composite levels (0–255)
#define NTSC_LEVEL_SYNC    0
#define NTSC_LEVEL_BLANK  77
#define NTSC_LEVEL_WHITE 255

void ntsc_init(void);
void ntsc_render_frame_step(void);

void ntsc_set_palette_bank(uint8_t bank);

#endif
