#include "engine_3d.h"
#include <cmath>

Engine3D::Engine3D(int w, int h) : width(w), height(h) {}

void Engine3D::setWorld(const std::vector<int> &w) {
    world = w;
}

std::vector<int> Engine3D::raycast(int px, int py, float angle, int maxDist) const {
    std::vector<int> hits;
    float dx = std::cos(angle);
    float dy = std::sin(angle);

    float x = px;
    float y = py;
    for (int d = 0; d < maxDist; ++d) {
        x += dx;
        y += dy;
        int ix = (int)x;
        int iy = (int)y;
        if (ix < 0 || iy < 0 || ix >= width || iy >= height) break;
        int idx = iy * width + ix;
        int block = world[idx];
        hits.push_back(block);
        if (block != 0) break; // stop at first solid block
    }
    return hits;
}
