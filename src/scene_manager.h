#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "geometry.h"
#include "globals.h"
#include "player.h"
#include "scene.h"
#include <vector>

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
