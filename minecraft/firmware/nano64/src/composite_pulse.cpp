#include "composite_pulse.h"
#include <tar.h>   // placeholder; real implementation would parse tar.gz
#include <fstream>

CompositePulse::CompositePulse(const std::string &path) : pulsePath(path) {}

bool CompositePulse::load() {
    // Minimal stub: just check file exists.
    std::ifstream f(pulsePath);
    return f.good();
}
