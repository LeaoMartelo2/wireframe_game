#include "scene_manager.h"
#include "include/lognest.h"

SceneManager::SceneManager() : player() {

    lognest_trace("[SceneManager] Scene manager created");

    current_scene = 0;

    add_scenes();
}

void SceneManager::end() {
}

void SceneManager::swap_scene(size_t scene_id) {

    current_scene = scene_id;

    scenes[current_scene]->start();
}

void SceneManager::update_current_scene() {
    scenes[current_scene]->update();

    if (IsKeyPressed(KEY_H)) {
        swap_scene(current_scene + 1);
        printf("%zu\n", current_scene);
    }
}
