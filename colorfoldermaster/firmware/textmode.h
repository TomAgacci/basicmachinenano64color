#pragma once
#include <Arduino.h>

extern const uint8_t TEXT_COLS;
extern const uint8_t TEXT_ROWS;

void textInit();
void textClear();
void textSetCursor(uint8_t x, uint8_t y);
void textPrint(const char* s);
void textPrintAt(uint8_t x, uint8_t y, const char* s);
void textScrollUp();

uint8_t textGetChar(uint8_t x, uint8_t y);
void textSetChar(uint8_t x, uint8_t y, uint8_t ch);
