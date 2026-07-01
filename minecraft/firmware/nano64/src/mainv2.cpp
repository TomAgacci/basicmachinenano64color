#include "basic_vm.h"
#include "video.h"
#include "input.h"
#include "engine_2d.h"
#include "engine_3d.h"
#include "composite_color.h"

int main() {
    Video video(128, 64);
    Input input;

    Engine2D e2d(64, 32);
    e2d.initFlatWorld();

    Engine3D e3d(64, 32);
    e3d.setWorld(e2d.getWorld());

    CompositeColor color(128, 64);

    BasicVM vm(video, input, e2d, e3d, color);

    vm.loadProgram("programs/microcraft.bas");
    while (true) {
        input.poll();
        vm.step();
        video.present();      // mono
        vm.presentColor();    // color mapping (for composite head)
    }
    return 0;
}
