🔥 PART 3 — Integrating into ntsc_core.c
Replace the active‑video pixel fetch:

c
pix = ntsc_framebuffer[line][x];
with:

c
pix = fetch_pixel_from_nano64(line, x);
And include the header:

c
#include "fetch_pixel_from_nano64.h"
