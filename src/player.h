#ifndef PLAYER_H_
#define PLAYER_H_

#include "models.h"
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

#define GROUND_PLATES 2

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
        Vector3 velocity;
        bool is_grounded;
        Vector3 viewmodel_pos;
        bool faceup;
        BoundingBox bounding_box;
        int ground_pcount;
        Ground ground_geometry[GROUND_PLATES];
        int geometry_count;
        Solid geometry[];

} Player;

void init_player(Player *player);

void move_player(Player *p);

void update_player(Player *player);

/*bool check_colision_test(Player *player, Vector3 floor_pos, Vector3 floor_size);*/

void draw_viewmodel(Player *player, Model viewmodel);

#endif // !PLAYER_H_
