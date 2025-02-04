#include "scene_manager.h"
#include "globals.h"
#include "include/lognest.h"

SceneManager::SceneManager() : player() {

    lognest_trace("[SceneManager] Scene manager created.");

    current_scene = 0;

    add_scenes();

    scenes[current_scene]->start();
}

SceneManager::~SceneManager() {
    for (size_t i = 0; i < scenes.size(); ++i) {
        delete scenes[i];
        lognest_debug(" ┗>[SceneManager] Cleared scene with ID: %zu", i);
    }
}

void SceneManager::end() {
}

void SceneManager::swap_scene(size_t scene_id) {

    if (scene_id >= scenes.size()) {
        lognest_error("[SceneManager] Attempted to load a unexisting scene. "
                      "Exiting. Tried to load ID: '%zu'",
                      scene_id);
        close_application = true;
        return;
    }

    lognest_trace("[SceneManager] Loading scene with ID: '%zu'", scene_id);

    if (scene_id != current_scene) {
        scenes[current_scene]->end();
    } else {
        lognest_warn("[SceneManager] Attemped to load a scene that is already loaded. Scene ID: '%zu'", scene_id);
    }

    current_scene = scene_id;

    scenes[current_scene]->start();
}

void SceneManager::update() {
    scenes[current_scene]->update();

    if (IsKeyPressed(KEY_H)) {
        swap_scene(current_scene + 1);
    }

    if (WindowShouldClose()) {
        close_application = true;
    }
}
