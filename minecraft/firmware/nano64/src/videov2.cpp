#include "video.h"
#include <iostream>

Video::Video(int w, int h) : width(w), height(h), fb(w * h, false) {}

void Video::clear() {
    std::fill(fb.begin(), fb.end(), false);
}

void Video::setPixel(int x, int y, bool on) {
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    fb[y * width + x] = on;
}

const std::vector<bool> &Video::getFramebuffer() const {
    return fb;
}

void Video::present() {
    std::cout << "\x1b[H";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (fb[y * width + x] ? '#' : ' ');
        }
        std::cout << "\n";
    }
}
