#include "scene_manager.h"

void SceneManager::add_scenes() {

    // setup each level specific data here;

    MainMenu *test_menu = new MainMenu;
    test_menu->scene_id = SCENE_MAINMENU;
    test_menu->parent = this; // Main Menu requires a pointer to its parent Scene Manager
    scenes.emplace_back(test_menu);

    Scene *test_level = new Scene;
    test_level->set_map("levels/test");
    test_level->player = &player;
    test_level->scene_id = SCENE_LEVEL_SAMPLE;
    test_level->start_pos = {150, 20, 50};
    scenes.emplace_back(test_level);

    Scene *level2 = new Scene;
    level2->set_map("levels/level2");
    level2->player = &player;
    level2->scene_id = SCENE_LEVEL_TEST2;
    level2->start_pos = {150, 20, 50};
    scenes.emplace_back(level2);
}
