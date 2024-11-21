#ifndef PLAYER_H_
#define PLAYER_H_

#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

typedef struct Player {
        Camera3D camera;
        Vector3 postition;
        Vector3 bounding_box_size;
        bool turn_A;
        bool turn_D;
        float move_speed;
        float cam_rot_speed;
        float cam_rol_scale;
        float gravity;

} Player;

void move_cam(Player *p);

void update_player(Player *player);

bool check_colision_test(Player *player, Model floor, Vector3 floor_pos);

#endif // !PLAYER_H_
