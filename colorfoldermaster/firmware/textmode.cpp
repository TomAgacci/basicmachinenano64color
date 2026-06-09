#include "textmode.h"

const uint8_t TEXT_COLS = 32;
const uint8_t TEXT_ROWS = 24;

static uint8_t textBuf[TEXT_COLS * TEXT_ROWS];
static uint8_t cursorX = 0;
static uint8_t cursorY = 0;

void textInit() {
  textClear();
}

void textClear() {
  for (uint16_t i = 0; i < TEXT_COLS * TEXT_ROWS; ++i) {
    textBuf[i] = ' ';
  }
  cursorX = cursorY = 0;
}

void textSetCursor(uint8_t x, uint8_t y) {
  if (x < TEXT_COLS) cursorX = x;
  if (y < TEXT_ROWS) cursorY = y;
}

void textPrint(const char* s) {
  while (*s) {
    char c = *s++;
    if (c == '\n') {
      cursorX = 0;
      if (++cursorY >= TEXT_ROWS) {
        textScrollUp();
        cursorY = TEXT_ROWS - 1;
      }
    } else {
      textSetChar(cursorX, cursorY, (uint8_t)c);
      if (++cursorX >= TEXT_COLS) {
        cursorX = 0;
        if (++cursorY >= TEXT_ROWS) {
          textScrollUp();
          cursorY = TEXT_ROWS - 1;
        }
      }
    }
  }
}

void textPrintAt(uint8_t x, uint8_t y, const char* s) {
  textSetCursor(x, y);
  textPrint(s);
}

void textScrollUp() {
  for (uint8_t y = 1; y < TEXT_ROWS; ++y) {
    for (uint8_t x = 0; x < TEXT_COLS; ++x) {
      textBuf[(y - 1) * TEXT_COLS + x] = textBuf[y * TEXT_COLS + x];
    }
  }
  for (uint8_t x = 0; x < TEXT_COLS; ++x) {
    textBuf[(TEXT_ROWS - 1) * TEXT_COLS + x] = ' ';
  }
}

uint8_t textGetChar(uint8_t x, uint8_t y) {
  if (x >= TEXT_COLS || y >= TEXT_ROWS) return ' ';
  return textBuf[y * TEXT_COLS + x];
}

void textSetChar(uint8_t x, uint8_t y, uint8_t ch) {
  if (x >= TEXT_COLS || y >= TEXT_ROWS) return;
  textBuf[y * TEXT_COLS + x] = ch;
}
