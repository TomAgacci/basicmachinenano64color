#pragma once
#include <vector>

class Engine2D {
public:
    Engine2D(int w, int h);
    void initFlatWorld();
    const std::vector<int> &getWorld() const;

private:
    int width, height;
    std::vector<int> world; // block IDs
};
