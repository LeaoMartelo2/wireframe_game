#include "globals.h"
#include "misc.h"
#include "scene_manager.h"

int main(void) {

    raylib_pre();

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    {
        raylib_config();

        SceneManager scene_manager;
        scene_manager.swap_scene(SCENE_MAINMENU);

        while (!close_application) {

            scene_manager.update();
        }

        scene_manager.end();
    }
    CloseWindow();

    raylib_post();

    return 0;
}
