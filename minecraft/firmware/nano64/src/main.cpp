#include "basic_vm.h"
#include "video.h"
#include "input.h"

int main() {
    Video video(128, 64);
    Input input;
    BasicVM vm(video, input);

    vm.loadProgram("programs/microcraft.bas");
    while (true) {
        input.poll();
        vm.step();
        video.present();
    }
    return 0;
}
