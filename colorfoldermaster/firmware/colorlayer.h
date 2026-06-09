#pragma once
#include <Arduino.h>

// Logical color per text cell (0–15)
void colorLayerInit();
void colorLayerSet(uint8_t x, uint8_t y, uint8_t colorIndex);
uint8_t colorLayerGet(uint8_t x, uint8_t y);

// Helper: given a grey (0–255), pick nearest palette index
uint8_t colorLayerIndexForGrey(uint8_t grey);
