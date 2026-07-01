#include "composite_color.h"
#include <vector>

CompositeColor::CompositeColor(int w, int h)
    : width(w), height(h), colors(w * h, 0) {}

void CompositeColor::mapMonoToColor(const std::vector<bool> &monoFb) {
    for (int i = 0; i < width * height && i < (int)monoFb.size(); ++i) {
        colors[i] = monoFb[i] ? 7 : 0; // simple: on=gray, off=black
    }
}
