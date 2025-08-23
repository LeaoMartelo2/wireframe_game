#ifndef DEBUG
#define LOGNEST_DISABLE_DEBUG
#endif // DEBUG

#define LOGNEST_FILE "latest.log"
#define LOGNEST_ALLOW_CRASH
#define LOGNEST_IMPLEMENTATION
#include "include/lognest.h"

#include "globals.h"
#include "misc.h"
#include "scene_manager.h"

int main(void) {

    lognest_debug("test\n");

    raylib_pre();

    InitWindow(1366, 768, "Wireframe Engine");
    {
        raylib_config();

        SceneManager scene_manager;

        while (!close_application) {

            scene_manager.update();
        }

        scene_manager.end();
    }
    CloseWindow();

    raylib_post();

    return 0;
}
