#include "engine_2d.h"
#include "engine_3d.h"
#include <iostream>

int main() {
    Engine2D e2d(64, 32);
    Engine3D e3d(64, 32);

    e2d.initFlatWorld();
    e3d.setWorld(e2d.getWorld());

    std::cout << "Engine initialized (2D + raycast 3D).\n";
    return 0;
}
