#include "sprites.h"

static Sprite sprites[MAX_SPRITES];

void spritesInit() {
  for (uint8_t i = 0; i < MAX_SPRITES; ++i) {
    sprites[i].x = 0;
    sprites[i].y = 0;
    sprites[i].tile = 0;
    sprites[i].color = 15;
    sprites[i].visible = false;
  }
}

void spritesUpdate() {
  // Placeholder: user/game logic can animate sprites here
}

Sprite* spriteGet(uint8_t i) {
  if (i >= MAX_SPRITES) return nullptr;
  return &sprites[i];
}
