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
        float forward_velocity;
        float sideways_velocity; /* should probably move this in to a Vector3 */
        float vertical_velocity;
        bool is_grounded;
        Vector3 viewmodel_pos;
        bool faceup;

} Player;

void move_cam(Player *p);

void update_player(Player *player);

bool check_colision_test(Player *player, Vector3 floor_pos, Vector3 floor_size);

void draw_viewmodel(Player *player, Model viewmodel);

#endif // !PLAYER_H_
