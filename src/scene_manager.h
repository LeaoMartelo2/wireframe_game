#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "geometry.h"
#include "player.h"
#include "scene.h"
#include <vector>

typedef enum : size_t {
    SCENE_MAINMENU = 0,
    SCENE_LEVEL_TEST1,
    SCENE_LEVEL_TEST2,
    SCENE_LEVEL_TEST3,
} LEVELS;

class SceneManager {
  private:
  public:
    std::vector<Scene *> scenes;
    Player player;
    size_t current_scene;

  public:
    SceneManager();
    ~SceneManager();

    void add_scenes();
    void end();

    void swap_scene(size_t scene_id);

    void update();
};

#endif // !SCENE_MANAGER_H_
