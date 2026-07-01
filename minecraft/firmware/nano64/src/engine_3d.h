#pragma once
#include <vector>

class Engine3D {
public:
    Engine3D(int w, int h);
    void setWorld(const std::vector<int> &w);
    std::vector<int> raycast(int px, int py, float angle, int maxDist) const;

private:
    int width, height;
    std::vector<int> world;
};
