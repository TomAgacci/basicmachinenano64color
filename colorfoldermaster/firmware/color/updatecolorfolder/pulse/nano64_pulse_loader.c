#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdint.h>
#include "ntsc_core/ntsc_core.h"
#include "ntsc_core/palettes.h"
#include "ntsc_core/framebuffer.h"

// ------------------------------------------------------------
//  PULSE LOADER FOR NANO64
//  Loads a single-line .pulse binary and installs the NTSC core
// ------------------------------------------------------------

// Maximum size of a .pulse binary line
#define PULSE_MAX 1024

// Simple hex decoder
static uint8_t hex2b(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

static uint8_t hexbyte(const char *p) {
    return (hex2b(p[0]) << 4) | hex2b(p[1]);
}

// ------------------------------------------------------------
//  Decompress PULSE DATA → RAM
//  (Your .pulse format is already "compressed", so this is a
//   direct hex → byte unpacker.)
// ------------------------------------------------------------
uint16_t pulse_unpack(const char *pulse_line, uint8_t *out, uint16_t maxlen) {
    const char *data = strstr(pulse_line, "DATA=");
    if (!data) return 0;

    data += 5; // skip "DATA="

    uint16_t count = 0;
    while (data[0] && data[1] && count < maxlen) {
        out[count++] = hexbyte(data);
        data += 2;
    }
    return count;
}

// ------------------------------------------------------------
//  Install NTSC core from .pulse binary
// ------------------------------------------------------------
void pulse_install_ntsc(const uint8_t *blob, uint16_t len) {
    // In your engine, the .pulse blob contains:
    // - LUTs
    // - Phase constants
    // - Palette banks
    // - Tile fetch metadata
    // - NTSC core metadata
    //
    // The NTSC core code itself is compiled into the firmware,
    // so the loader only needs to:
    //   1. Load palette banks
    //   2. Load any runtime constants
    //   3. Initialize NTSC engine

    // Example: first 4 bytes = PHASE_INC override
    uint32_t ph =
        ((uint32_t)blob[0] << 24) |
        ((uint32_t)blob[1] << 16) |
        ((uint32_t)blob[2] << 8)  |
        ((uint32_t)blob[3]);

    // If nonzero, override PHASE_INC
    if (ph != 0) {
        // You can store this in a global if you want runtime override
        // ntsc_phase_inc = ph;
    }

    // Next: palette banks (256 * 3 * NBANKS bytes)
    // You can copy them into RAM if you want runtime palette switching.
    // But since your palettes live in PROGMEM, we skip this step.

    // Initialize NTSC engine
    ntsc_init();
}

// ------------------------------------------------------------
//  PUBLIC API: load a .pulse line
// ------------------------------------------------------------
void pulse_load_line(const char *line) {
    static uint8_t blob[512];

    uint16_t len = pulse_unpack(line, blob, sizeof(blob));
    if (len == 0) return;

    pulse_install_ntsc(blob, len);
}

