#include "scene.h"
#include "scene_manager.h"
#include <memory>

void SceneManager::add_scenes() {

    // setup each level specific data here;

    std::shared_ptr test_level = std::make_shared<Scene>();
    test_level->set_map("levels/test");
    test_level->player = &player;
    test_level->scene_id = LEVEL_TEST;
    test_level->start_pos = {150, 20, 50};
    scenes.emplace_back(test_level);

    std::shared_ptr level2 = std::make_shared<Scene>();
    level2->set_map("levels/level2");
    level2->player = &player;
    level2->scene_id = LEVEL_SAMPLE;
    level2->start_pos = {150, 20, 50};
    scenes.emplace_back(level2);
}
