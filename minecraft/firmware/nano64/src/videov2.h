#pragma once
#include <vector>

class Video {
public:
    Video(int w, int h);
    void clear();
    void setPixel(int x, int y, bool on);
    void present();

    const std::vector<bool> &getFramebuffer() const;

private:
    int width, height;
    std::vector<bool> fb;
};
