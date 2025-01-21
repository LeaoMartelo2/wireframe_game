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
