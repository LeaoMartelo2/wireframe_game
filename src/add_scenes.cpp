#include "scene.h"
#include "scene_manager.h"

void SceneManager::add_scenes() {

    // setup each level specific data here;

    MainMenu *test_menu = new MainMenu;
    test_menu->scene_id = SCENE_MAINMENU;
    test_menu->parent = this; // Main Menu requires a pointer to its parent Scene Manager
    scenes.emplace_back(test_menu);

    Scene *test_level = new Scene;
    test_level->set_map("levels/level1");
    test_level->player = &player;
    test_level->parent = this;
    test_level->scene_id = SCENE_LEVEL_TEST1;
    scenes.emplace_back(test_level);

    Scene *level2 = new Scene;
    level2->set_map("levels/level2");
    level2->player = &player;
    level2->parent = this;
    level2->scene_id = SCENE_LEVEL_TEST2;
    scenes.emplace_back(level2);

    Scene *level3 = new Scene;
    level3->set_map("levels/level3");
    level3->player = &player;
    level3->parent = this;
    level3->scene_id = SCENE_LEVEL_TEST3;
    scenes.emplace_back(level3);
}
