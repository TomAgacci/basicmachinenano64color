#include "composite_color.h"
#include "composite_pulse.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: composite_main <pulse_file>\n";
        return 1;
    }

    CompositePulse pulse(argv[1]);
    if (!pulse.load()) {
        std::cerr << "Failed to load pulse\n";
        return 1;
    }

    CompositeColor color(128, 64);
    // In a real system, we'd feed Nano64 framebuffer into CompositeColor.
    // Here we just show that the pulse is recognized.
    std::cout << "Composite color head running with pulse: " << argv[1] << "\n";
    return 0;
}
