#ifndef PLAYER_H_
#define PLAYER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rcamera.h"
#include "geometry.h"

typedef struct Player {
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
        } misc;

} Player;

void player_setup(Player *player);

void player_set_collision_map(Player *player, Geometry_Array *map_geometry);

void player_update(Player *player);

void player_draw3D(Player *player);

void player_draw_hud(Player *player);

void player_debug(Player *player);

#endif // !PLAYER_H_
