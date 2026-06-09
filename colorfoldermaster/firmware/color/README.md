# Nano64 NTSC Core (Drop‑in Module)

This folder contains a self‑contained NTSC composite encoder for the Nano64
color build. It generates a 1 Vpp NTSC‑like signal on PORTD using a simple
resistor ladder and a couple of passive parts.

## Hardware

- MCU: ATmega328P @ 16 MHz (Arduino Nano)
- Video DAC: PORTD (D0–D7) → 8‑bit R‑2R ladder
- Analog chain:

  PORTD → R‑2R → 330 Ω → node A  
  node A → 220 pF to GND  
  node A → 300 Ω → RCA center  
  RCA shield → GND

TV input is 75 Ω; the 300 Ω series resistor scales 0–5 V to 0–1 V.

## Levels

- Sync: 0
- Blanking: 77
- White: 255

These map to 0 V, 0.3 V, and 1.0 V at the TV.

## Usage

1. Add this folder to your repo.
2. Wire PORTD to your existing Nano64 color ladder.
3. In your main code:

   ```c
   #include "ntsc_core.h"
   #include "framebuffer.h"

   void setup() {
       ntsc_init();
       // fill ntsc_framebuffer and palettes
   }

   void loop() {
       ntsc_render_frame_step();
       // game / engine logic here
   }
