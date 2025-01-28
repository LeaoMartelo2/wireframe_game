#include "misc.h"
#include "scene_manager.h"

int main(void) {

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

    raylib_post();

    return 0;
}
