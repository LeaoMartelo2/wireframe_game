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
    size_t get_scene_id_by_level(const char *levelname);
    ;
    size_t add_scene_by_name(const char *filename);
    void end();

    void swap_scene(size_t scene_id);

    void update();
};

#endif // !SCENE_MANAGER_H_
