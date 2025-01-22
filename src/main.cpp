#define LOGNEST_IMPLEMENTATION
#include "include/lognest.h"
#include "misc.h"
#include "scene_manager.h"

int main(void) {

    lognest_trace("|=====================-Wireframe Engine Start-=====================|\n");

    raylib_setup();
    InitAudioDevice();

    DisableCursor();

    {
        SceneManager scene_manager;
        // the first scene id should normally be 0;
        scene_manager.swap_scene(0);

        while (!WindowShouldClose()) {

            scene_manager.update_current_scene();
        }

        scene_manager.end();

        CloseWindow();
    }

    lognest_trace("[Raylib] Window closed.");

    lognest_trace("|=====================-Wireframe Engine End-=======================|\n");

    return 0;
}
