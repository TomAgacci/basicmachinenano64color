#pragma once
#include <string>
#include "video.h"
#include "input.h"
#include "engine_2d.h"
#include "engine_3d.h"
#include "composite_color.h"

class BasicVM {
public:
    BasicVM(Video &v, Input &i, Engine2D &e2d, Engine3D &e3d, CompositeColor &c);
    void loadProgram(const std::string &path);
    void step();
    void presentColor();

private:
    Video &video;
    Input &input;
    Engine2D &engine2d;
    Engine3D &engine3d;
    CompositeColor &color;
    std::string program;
    size_t pc;

    void runMinecraftFrame();
};
