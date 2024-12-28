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
        } collision;

        struct {
                Model viewmodel;
                Vector3 viewmodel_pos;
        } hud;

        struct {
                bool noclip;
        } misc;

} Player;

void player_setup(Player *player);

void update_player(Player *player);

inline void player_debug(Player *player) {

    DrawRectangle(5, 5, 300, 205, ColorAlpha(GetColor(0x001100FF), 0.7f));

    DrawText(TextFormat("Position:\nX: %.2f, Y: %.2f, Z: %.2f\n"
                        "Input:\n -> Forward: %f\n -> Sideways: %f\n -> Upwards: %f\n"
                        "Velocity:\n -> Forward: %.2f\n -> Sideways: %.2f\n",
                        player->pos.x, player->pos.y, player->pos.z,
                        player->input.forwards, player->input.sideways, player->input.up_down,
                        player->velocity.forwards, player->velocity.sideways),
             10, 10,
             20, WHITE);
}

#endif // !PLAYER_H_
