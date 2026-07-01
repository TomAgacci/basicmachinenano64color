#pragma once
#include <vector>

class CompositeColor {
public:
    CompositeColor(int w, int h);
    void mapMonoToColor(const std::vector<bool> &monoFb);

private:
    int width, height;
    std::vector<int> colors; // 0–15 color indices
};
