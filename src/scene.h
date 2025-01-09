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
    void start();
    void update();
    void end();

    void loadmap();

  private:
    // private methods
};

#endif // !SCENE_H_
