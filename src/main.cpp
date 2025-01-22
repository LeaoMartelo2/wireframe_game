#define LOGNEST_IMPLEMENTATION
#include "include/lognest.h"
#include "misc.h"
#include "scene_manager.h"

int main(void) {

    lognest_trace("|=====================-Wireframe Engine Start-=====================|\n");

    raylib_pre();
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    {
        raylib_config();
        DisableCursor();

        SceneManager scene_manager;
        scene_manager.swap_scene(LEVEL_TEST);

        while (!WindowShouldClose()) {

            scene_manager.update_current_scene();
        }

        scene_manager.end();
    }
    CloseWindow();
    lognest_trace("[Raylib] Window closed.");

    lognest_trace("|=====================-Wireframe Engine End-=======================|\n");

    return 0;
}
