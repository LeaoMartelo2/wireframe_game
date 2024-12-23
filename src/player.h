#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"
#include "models.h"

typedef struct Player {
        struct {
                Camera3D player_cam;
                float mouse_sens;
                float camera_tilt;
        } camera;

        struct {
                Vector3 pos;
                Vector3 velocity;
                float move_speed;
                float gravity;
                bool is_grounded;
                Vector2 input;
        } movement;

        struct {
                Vector3 bounding_box_size;
                BoundingBox bounding_box;
        } collision;

        struct {
                Model viewmodel;
                Vector3 viewmodel_pos;
        } hud;

        struct {
                bool turn_A;
                bool turn_D;
        } misc;

} Player;

void player_setup(Player *player);

void move_player(Player *p);

void update_player(Player *player);

void draw_viewmodel(Player *player);

#endif // !PLAYER_H_
