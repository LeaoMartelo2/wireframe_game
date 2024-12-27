#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"

typedef struct Player {
        Camera3D camera;
        struct {
                float mouse_sens;
                float camera_tilt;
                bool turn_A;
                bool turn_D;

        } camera_misc;

        Vector3 pos;
        Vector3 velocity;
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
                Vector3 bounding_box_size;
                BoundingBox bounding_box;
        } collision;

        struct {
                Model viewmodel;
                Vector3 viewmodel_pos;
        } hud;

} Player;

void player_setup(Player *player);

void update_player(Player *player);

#endif // !PLAYER_H_
