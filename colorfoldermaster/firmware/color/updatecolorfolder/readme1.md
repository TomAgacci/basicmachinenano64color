Readme

✅ PART 2 — Adapting NTSC core to your real Nano64 framebuffer format
Your repo uses a packed tile/character system, not a raw 256×192 framebuffer.

Your format:

Screen is divided into 8×8 tiles

Each tile has:

A character index

A color index

Pixel color = palette[color_index]

So we replace:

c
pix = ntsc_framebuffer[line][x];
with:

c
pix = fetch_pixel_from_nano64(line, x);
Here is the correct drop‑in function:
