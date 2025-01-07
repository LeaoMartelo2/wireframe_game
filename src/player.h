#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"
#include "geometry.h"

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
    float gravity;
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
        Geometry_Array map_geometry;
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

    void update();
    void draw();
    void draw_hud();
    BoundingBox calculate_boundingbox();

    Vector3 get_forward();
    Vector3 get_up();
    Vector3 get_right();

    void debug();
    void debug_3d();

  private:
    // private variables
  private:
    // private methods
    void get_input();
    void calculate_velocity();
    void move_forward(float distance);
    void move_right(float distance);
    void move_vertical(float distance);
    void move();
    void update_camera();
    void update_viewmodel();
    void draw_viewmodel();
};

#endif // !PLAYER_H_
