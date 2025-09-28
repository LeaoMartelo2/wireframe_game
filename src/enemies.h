#ifndef ENEMIES_H
#define ENEMIES_H

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include "player.h"
#include <stdint.h>
#include <vector>

const Vector3 ROTATION_AXIS = {0, 1, 0};
const Vector3 SCALE = {1, 1, 1};
const float NO_ROTATION = 0;
const float WIREFRAME_WIDTH = 0.3f;
const Color WIREFRAME_COLOR = BLACK;
const Color MODEL_COLOR = WHITE;

class Enemy;

typedef struct Enemy_body_parts {

    Model model;
    Vector3 offset;
    bool face_player;
    bool has_wireframe = true;
    Vector3 custom_scale = {1, 1, 1};
    float angle_offset = 0.0f;

    bool update_pos;
    void (*update_pos_function)(Enemy_body_parts *, Vector3);
    float distance;

    bool custom_drawing = false;
    void (*custom_drawing_function)(const Enemy_body_parts *, const Vector3, const float);

} Enemy_body_parts;

class Enemy {

  public:
    struct {
        Vector3 pos;
        Vector3 size;
    } hitbox;

    std::vector<Enemy_body_parts *> body_models;

    Vector3 look_pos;

    Vector3 forward;

    float angle = 0.0f;
    float turning_speed = 0.0f;

    Enemy();
    ~Enemy();

    Vector3 get_forward();
    Vector3 get_up();
    Vector3 get_right();

    void load();
    bool loaded = false;

    void update(GenericPlayerData_share player);

    void draw();
};

#endif // !ENEMIES_H_
