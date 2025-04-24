#ifndef SCENE_H_
#define SCENE_H_

#include "collision.h"
#include "player.h"
#include <string.h>
#include <string>
#include <vector>

class SceneManager;
// this needs forward declaration, thanks C++

class Scene {
  public:
    SceneManager *parent;

    std::vector<Geometry> map_geometry;
    std::vector<Floor> map_floor;
    std::vector<Trigger> map_trigger;

    std::vector<Collider *> map_colliders;

    Player *player;

    size_t scene_id;

    std::string map_file;

    Vector3 start_pos;
    Vector3 looking_at;

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

    /*virtual void update_map_triggers(void);*/
    /*virtual void debug_draw_map_triggers();*/

    /*virtual void draw_map_geometry(void);*/
    /*virtual void draw_map_floor(void);*/

    virtual void draw_scene_colliders(void);
};

class MainMenu : public Scene {

  private:
    Camera3D mm_camera;
    Model menu_model;
    Music menu_music;
    Sound menu_click;
    Texture2D mmenu_settings;

  public:
    MainMenu();
    ~MainMenu();

    void start() override;
    void end() override;
    void update() override;

  public:
    SceneManager *parent;
};

#endif // !SCENE_H_
