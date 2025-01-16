#define LOGNEST_IMPLEMENTATION
#include "include/lognest.h"
#include "misc.h"
#include "scene.h"

int main(void) {

    lognest_trace("|=====================-Wireframe Engine Start-=====================|\n");

    raylib_setup();
    InitAudioDevice();

    DisableCursor();

    {
        Scene scene;

        scene.start("levels/test");

        while (!WindowShouldClose()) {

            scene.update();
        }

        scene.end();

        CloseWindow();
    }

    lognest_trace("[Raylib] Window closed.");

    lognest_trace("|=====================-Wireframe Engine End-=======================|\n");

    return 0;
}
