#pragma once
#include <string>
#include "video.h"
#include "input.h"

class BasicVM {
public:
    BasicVM(Video &v, Input &i);
    void loadProgram(const std::string &path);
    void step();

private:
    Video &video;
    Input &input;
    std::string program;
    size_t pc;

    void runMicrocraftFrame();
};
