#include "scene_manager.h"

void SceneManager::add_scenes() {

    // here you create your scenes, load their maps, and add them to the scene manager

    {
        Scene test_level;
        test_level.start("levels/test");
        test_level.player = &player;
        test_level.scene_id = 0;
        scenes.emplace_back(test_level);

        Scene test_2;
        test_2.start("levels/test");
        test_2.player = &player;
        test_level.scene_id = 1;
        scenes.emplace_back(test_2);
    }
}
