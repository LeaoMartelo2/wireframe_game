#ifndef SCENE_H_
#define SCENE_H_

#include "geometry.h"
#include "player.h"
#include <vector>

class Scene {
  public:
    std::vector<Geometry> map_geometry;
    std::vector<Floor> map_floor;
    Player *player;

    size_t scene_id;

  private:
    // private members

  public:
    Scene();
    ~Scene();
    void start(const char *filename);
    void update(void);
    void end(void);

    void loadmap(const char *filename);

  private:
    // private methods

    void draw_map_geometry(void);
    void draw_map_floor(void);
};

#endif // !SCENE_H_
