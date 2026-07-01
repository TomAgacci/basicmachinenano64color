#include "basic_vm.h"
#include <fstream>
#include <sstream>

BasicVM::BasicVM(Video &v, Input &i) : video(v), input(i) {}

void BasicVM::loadProgram(const std::string &path) {
    std::ifstream f(path);
    std::stringstream buf;
    buf << f.rdbuf();
    program = buf.str();
    pc = 0;
}

void BasicVM::step() {
    // Extremely minimal “VM”: just calls a hardcoded Microcraft loop.
    // In a real system this would parse and execute BASIC.
    runMicrocraftFrame();
}

void BasicVM::runMicrocraftFrame() {
    // Placeholder: in a real VM, this would be the BASIC program logic.
    // Here we just clear and draw a dummy world.
    video.clear();
    for (int y = 24; y < 64; ++y) {
        for (int x = 0; x < 128; ++x) {
            video.setPixel(x, y, true);
        }
    }
}
