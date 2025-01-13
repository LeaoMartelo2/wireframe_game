#ifndef SCENE_H_
#define SCENE_H_

#include "geometry.h"
#include "player.h"
#include <vector>

class Scene {
  public:
    std::vector<Geometry> map_geometry;
    Player player;

  private:
    // private members

  public:
    Scene();
    ~Scene();
    void start(const char *filename);
    void update();
    void end();

    void loadmap(const char *filename);

  private:
    // private methods

    void draw_map_geometry();
};

#endif // !SCENE_H_
