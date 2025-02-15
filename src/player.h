#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"
#include "geometry.h"
#include <vector>

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

    Vector3 pos;
    float acc_rate;
    float move_speed;
    float side_speed;
    float air_boost;
    float gravity;
    float max_acell;
    bool is_grounded;

    struct {
        float forwards;
        float sideways;
        float up_down;
    } input;

    struct {
        float forwards;
        float sideways;
        float vertical;
    } velocity;

    struct {
        Vector3 bounding_box_size;
        BoundingBox bounding_box;
    } collision;

    struct {
        Model model;
        Vector3 viewmodel_pos;
    } viewmodel;

    struct {
        bool show_debug;
        bool noclip;
        bool no_gravity;
    } misc;

  public:
    // public methods
    Player();
    Player(const Player &other);
    /*~Player();*/

    void update(std::vector<Geometry> &map_geometry, std::vector<Floor> &map_floor);
    void draw(void);
    void draw_hud(void);
    BoundingBox calculate_boundingbox(void);
    bool check_collision_geometry(std::vector<Geometry> &map_geometry, Vector3 cube_pos);
    bool check_collision_floor(std::vector<Floor> &map_floor);

    Vector3 get_forward(void);
    Vector3 get_up(void);
    Vector3 get_right(void);

    void debug(void);
    void debug_3d(void);

  private:
    // private variables
  private:
    // private methods
    void get_input(void);
    void calculate_velocity(void);
    Vector3 move_forward(float distance);
    Vector3 move_right(float distance);
    Vector3 move_vertical(float distance);
    Vector3 new_pos(float distance_forward, float distance_right);
    void update_gravity();
    void move(std::vector<Geometry> &map_geometry, std::vector<Floor> &map_floor);
    void update_camera(void);
    void update_viewmodel(void);
    void draw_viewmodel(void);
};

#endif // !PLAYER_H_
