#include "scene_manager.h"
#include "include/lognest.h"

SceneManager::SceneManager() : player() {

    lognest_trace("[SceneManager] Scene manager created");

    current_scene = 0;

    add_scenes();
}

void SceneManager::end() {
}

void SceneManager::update_current_scene() {
    scenes[current_scene].update();

    if (IsKeyPressed(KEY_H)) {
        current_scene++;
        printf("%zu\n", current_scene);
    }
}
