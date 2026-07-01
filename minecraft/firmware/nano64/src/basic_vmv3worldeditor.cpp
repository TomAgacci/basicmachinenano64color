void BasicVM::step() {
    handleInput();
    runMinecraftFrame();
}

void BasicVM::handleInput() {
    // Stub: wire to Input when you have real key handling.
    // For now, no movement; just static world.
}

void BasicVM::runMinecraftFrame() {
    video.clear();

    const auto &world = engine2d.getWorld();
    for (int y = 0; y < worldH; ++y) {
        for (int x = 0; x < worldW; ++x) {
            int block = world[y * worldW + x];
            if (block != 0) {
                int sx = x * 2;
                int sy = y * 2;
                video.setPixel(sx,     sy,     true);
                video.setPixel(sx + 1, sy,     true);
                video.setPixel(sx,     sy + 1, true);
                video.setPixel(sx + 1, sy + 1, true);
            }
        }
    }

    // Draw player as a 2×2 square
    int sx = px * 2;
    int sy = py * 2;
    video.setPixel(sx,     sy,     true);
    video.setPixel(sx + 1, sy,     true);
    video.setPixel(sx,     sy + 1, true);
    video.setPixel(sx + 1, sy + 1, true);
}
