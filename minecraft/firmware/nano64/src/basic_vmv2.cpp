#include "basic_vm.h"
#include <fstream>
#include <sstream>

BasicVM::BasicVM(Video &v, Input &i, Engine2D &e2d, Engine3D &e3d, CompositeColor &c)
    : video(v), input(i), engine2d(e2d), engine3d(e3d), color(c), pc(0) {}

void BasicVM::loadProgram(const std::string &path) {
    std::ifstream f(path);
    std::stringstream buf;
    buf << f.rdbuf();
    program = buf.str();
    pc = 0;
}

void BasicVM::step() {
    // Placeholder: instead of parsing BASIC, we call a “Minecraft frame”.
    runMinecraftFrame();
}

void BasicVM::runMinecraftFrame() {
    video.clear();

    // Use Engine2D world to draw blocks
    const auto &world = engine2d.getWorld();
    int w = 64, h = 32;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int block = world[y * w + x];
            if (block != 0) {
                // Scale 64x32 world into 128x64 screen (2x)
                video.setPixel(x * 2,     y * 2,     true);
                video.setPixel(x * 2 + 1, y * 2,     true);
                video.setPixel(x * 2,     y * 2 + 1, true);
                video.setPixel(x * 2 + 1, y * 2 + 1, true);
            }
        }
    }

    // Simple raycast “view” using Engine3D (not drawn yet, but available)
    auto hits = engine3d.raycast(32, 16, 0.0f, 32);
    (void)hits; // you can use this to drive a HUD or pseudo-3D later
}

void BasicVM::presentColor() {
    // Grab mono framebuffer and map to color
    // For this stub, we reconstruct mono from Video by re-rendering.
    // In a real system, Video would expose its framebuffer.
    // Here we assume a 128x64 mono buffer of “on” pixels for blocks.

    std::vector<bool> monoFb(128 * 64, false);
    // Very rough: mark all pixels as off; in a real implementation,
    // Video would give us the actual framebuffer.
    // This is just to show the wiring:
    color.mapMonoToColor(monoFb);
}
