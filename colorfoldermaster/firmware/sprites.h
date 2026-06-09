#pragma once
#include <Arduino.h>

struct Sprite {
  int8_t  x;
  int8_t  y;
  uint8_t tile;
  uint8_t color;
  bool    visible;
};

const uint8_t MAX_SPRITES = 8;

void spritesInit();
void spritesUpdate();
Sprite* spriteGet(uint8_t i);
