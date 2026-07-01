void BasicVM::runMinecraftFrame() {
    video.clear();

    const auto &world = engine2d.getWorld();
    for (int y = 0; y < worldH; ++y) {
        for (int x = 0; x < worldW; ++x) {
            int block = world[y * worldW + x];
            if (block != 0) {
                int sx = x * 2;
                int sy = y * 2 + 16; // shift down to leave top for 3D
                video.setPixel(sx,     sy,     true);
                video.setPixel(sx + 1, sy,     true);
                video.setPixel(sx,     sy + 1, true);
                video.setPixel(sx + 1, sy + 1, true);
            }
        }
    }

    // Player
    int sx = px * 2;
    int sy = py * 2 + 16;
    video.setPixel(sx,     sy,     true);
    video.setPixel(sx + 1, sy,     true);
    video.setPixel(sx,     sy + 1, true);
    video.setPixel(sx + 1, sy + 1, true);

    // Simple raycast “view” at top 16 pixels
    int screenCols = 64;
    for (int col = 0; col < screenCols; ++col) {
        float angle = -0.5f + (col / (float)screenCols); // -0.5..+0.5
        auto hits = engine3d.raycast(px, py, angle, 32);
        int height = 0;
        if (!hits.empty() && hits.back() != 0) {
            height = 8; // fixed height for any hit
        }
        for (int h = 0; h < height; ++h) {
            int vx = col * 2;
            int vy = 8 - h;
            video.setPixel(vx,     vy, true);
            video.setPixel(vx + 1, vy, true);
        }
    }
}
