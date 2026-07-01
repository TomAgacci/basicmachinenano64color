#include "engine_2d.h"

Engine2D::Engine2D(int w, int h) : width(w), height(h), world(w * h, 0) {}

void Engine2D::initFlatWorld() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            if (y > height / 2) world[idx] = 1;      // dirt
            else if (y == height / 2) world[idx] = 3; // grass
            else world[idx] = 0;                     // air
        }
    }
}

const std::vector<int> &Engine2D::getWorld() const {
    return world;
}
