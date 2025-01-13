#define LOGNEST_IMPLEMENTATION
#include "include/lognest.h"
#include "misc.h"
#include "scene.h"

int main(void) {

    lognest_trace("|=====================-Wireframe Engine Start-=====================|");

    raylib_setup();
    InitAudioDevice();

    DisableCursor();

    {
        Scene scene;

        scene.start("src/test_map.json");

        while (!WindowShouldClose()) {

            scene.update();
        }

        scene.end();

        CloseWindow();
    }

    lognest_trace("[Wireframe] Window closed.");

    return 0;
}
