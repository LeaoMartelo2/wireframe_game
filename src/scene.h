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

    std::string map_file;

    Vector3 start_pos;

  private:
    // private members

  public:
    Scene();
    ~Scene();
    void start();
    void update(void);
    void end(void);

    void set_map(const char *filename);

    void loadmap(const char *filename);

  private:
    // private methods

    void draw_map_geometry(void);
    void draw_map_floor(void);
};

class MainMenu : public Scene {

  private:
    Camera2D camera;

  public:
    MainMenu();
    ~MainMenu();

    void start(void);
    void update(void);

  public:
    size_t scene_id;
};

#endif // !SCENE_H_
