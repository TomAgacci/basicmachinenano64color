#include "input.h"
#include <iostream>

void Input::poll() {
    // Minimal stub: no real-time input, just placeholder.
    // In a real build, this would read keyboard or GPIO.
}

bool Input::isKeyDown(Key k) const {
    // Always false in this stub.
    return false;
}
