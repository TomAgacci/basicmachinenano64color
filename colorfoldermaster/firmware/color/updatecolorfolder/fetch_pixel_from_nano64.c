#include "../colorfoldermaster/colorfoldermaster.h"

// Nano64 tilemap:
// tilemap[y][x] = index into character set
// colormap[y][x] = color index (0–63)

extern uint8_t tilemap[24][32];
extern uint8_t colormap[24][32];
extern uint8_t charset[256][8];   // 256 chars × 8 rows

uint8_t fetch_pixel_from_nano64(uint16_t line, uint16_t x) {
    // Convert pixel coordinates → tile coordinates
    uint8_t tile_y = line >> 3;   // /8
    uint8_t tile_x = x >> 3;      // /8

    if (tile_y >= 24 || tile_x >= 32)
        return 0;

    uint8_t char_index  = tilemap[tile_y][tile_x];
    uint8_t color_index = colormap[tile_y][tile_x];

    // Row inside the character
    uint8_t row = line & 7;

    // Fetch 8‑bit bitmap row
    uint8_t bits = charset[char_index][row];

    // Column inside the tile
    uint8_t col = x & 7;

    // Extract bit (MSB first)
    uint8_t bit = (bits >> (7 - col)) & 1;

    // If bit=0 → background color 0
    if (!bit) return 0;

    // If bit=1 → tile color
    return color_index;
}
