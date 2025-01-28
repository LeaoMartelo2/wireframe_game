#include "scene.h"
#include "scene_manager.h"

void SceneManager::add_scenes() {

    // setup each level specific data here;

    Scene *test_level = new Scene;
    test_level->set_map("levels/test");
    test_level->player = &player;
    test_level->scene_id = LEVEL_TEST;
    test_level->start_pos = {150, 20, 50};
    scenes.emplace_back(test_level);

    Scene *level2 = new Scene;
    level2->set_map("levels/level2");
    level2->player = &player;
    level2->scene_id = LEVEL_SAMPLE;
    level2->start_pos = {150, 20, 50};
    scenes.emplace_back(level2);

    Menu *test_menu = new Menu;
    test_menu->scene_id = 2;
    scenes.emplace_back(test_menu);
}
