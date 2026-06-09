#!/bin/bash
#//Needs editing

set -e

echo "=== Nano64 Deluxe Engine Build ==="

avr-gcc \
  -mmcu=atmega328p \
  -Os \
  -DF_CPU=16000000UL \
  -I./src \
  -I./src/audio \
  -I./src/synth \
  -I./src/drums \
  -I./src/fm \
  -I./src/pulse \
  src/main.c \
  src/ntsc_core.c \
  src/palettes.c \
  src/audio/audio.c \
  src/synth/synth.c \
  src/drums/drums.c \
  src/fm/fm.c \
  src/pulse/pulse_music.c \
  -o nano64.elf

avr-objcopy -O ihex nano64.elf nano64.hex

echo "Build complete: nano64.hex"
