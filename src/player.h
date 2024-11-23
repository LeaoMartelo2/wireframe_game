#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"
#include "models.h"

#define GROUND_PLATES 10

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
        Model viewmodel;
        bool faceup;
        BoundingBox bounding_box;
        int ground_pcount;
        Ground ground_geometry[GROUND_PLATES];
        int geometry_count;
        Solid geometry[100];

} Player;

void init_player(Player *player);

void move_player(Player *p);

void update_player(Player *player);

/*bool check_colision_test(Player *player, Vector3 floor_pos, Vector3 floor_size);*/

void draw_viewmodel(Player *player);

#endif // !PLAYER_H_
