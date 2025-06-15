#ifndef SCENE_H_
#define SCENE_H_

#include "collision.h"
#include "doors.h"
#include "gui.h"
#include "items.h"
#include "player.h"
#include <string.h>
#include <string>
#include <vector>

class SceneManager;
// this needs forward declaration, thanks C++

class Scene {
  public:
    SceneManager *parent;

    std::vector<Collider> map_colliders;

    std::vector<Door> map_doors;

    std::vector<DroppedItem> map_items;

    std::vector<DroppedKey> map_keys;

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
    virtual void paused_update(void);
    virtual void end(void);

    virtual void set_map(const char *filename);

    virtual void loadmap(const char *filename);

  private:
    virtual void draw_pause_menu();

    virtual void draw_scene_colliders(void);

    virtual void update_scene_doors();
    virtual void draw_scene_doors();

    virtual void update_scene_items();
    virtual void draw_scene_items();

    virtual void update_scene_keys();
    virtual void draw_scene_keys();
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
