#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"
#include "../raylib/rlgl.h"
#include "collision.h"
#include "doors.h"
#include "items.h"
#include <vector>

typedef enum PLAYER_ITEMS {
    ITEM_EMPTY = 0,
    ITEM_SHOTGUN,
    ITEM_AXE,

} PLAYER_ITEMS;

class Player {
    // public variables
  public:
    Camera3D camera;
    struct {
        float mouse_sens;
        float camera_tilt;
        bool turn_A;
        bool turn_D;

    } camera_misc;

    float acc_rate;
    float move_speed;
    float side_speed;
    float jump_speed;
    float gravity;

    Collider collider;
    Vector3 velocity;
    bool is_grounded;

    struct {
        float forwards;
        float sideways;
        float up_down;
    } input;

    struct {
        Model model;
        Vector3 viewmodel_pos;
    } viewmodel;

    struct {
        int selected_item;

        Shotgun shotgun;
        Axe axe;

    } items;

    struct {
        long health;
        long ammo;
    } gameplay;

    struct {
        bool show_debug;
        bool noclip;
        bool no_gravity;
    } misc;

    Player();

    void update(const std::vector<Collider> &map_colliders, const std::vector<Door> &map_doors);
    void draw(void);
    void draw_hud(void);
    BoundingBox calculate_boundingbox(void);

    Vector3 get_forward(void);
    Vector3 get_up(void);
    Vector3 get_right(void);

    void set_pos(Vector3 new_pos);
    void set_looking_at(Vector3 new_pos);

    void damage(long ammount);
    void give_ammo(long ammount);

    void debug(void);
    void debug_3d(void);

  private:
    // private methods
    void get_input(void);
    void update_gravity();
    void jump();
    void move(const std::vector<Collider> &map_colliders, const std::vector<Door> &map_doors);

    void camera_yaw(float angle);
    void camera_pitch(float angle);
    void update_camera(void);
    void update_viewmodel(void);
    void draw_viewmodel(void);
};

#endif // !PLAYER_H_
