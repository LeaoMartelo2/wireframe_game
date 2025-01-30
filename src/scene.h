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
    virtual ~Scene();
    virtual void start();
    virtual void update(void);
    virtual void end(void);

    virtual void set_map(const char *filename);

    virtual void loadmap(const char *filename);

  private:
    // private methods

    virtual void draw_map_geometry(void);
    virtual void draw_map_floor(void);
};

class MainMenu : public Scene {

  private:
    Camera3D mm_camera;
    Model menu_model;

  public:
    MainMenu();
    ~MainMenu();

    void start() override;
    void update() override;

  public:
    size_t scene_id;
};

#endif // !SCENE_H_
