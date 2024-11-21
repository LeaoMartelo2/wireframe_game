#include "player.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

void player_move_forward(Player *player, float distance) {
    /* direction its facing */
    Vector3 forward = Vector3Normalize(Vector3Subtract(player->camera.target, player->camera.position));

    /* project vector in to world plane */
    forward.y = 0;
    forward = Vector3Normalize(forward);

    /* scale by distance */
    forward = Vector3Scale(forward, distance);

    player->postition = Vector3Add(player->postition, forward);
    player->camera.position = player->postition;
    player->camera.target = Vector3Add(player->camera.target, forward);
}

void player_move_right(Player *player, float distance) {

    Vector3 forward = Vector3Normalize(Vector3Subtract(player->camera.target, player->camera.position));

    Vector3 up = Vector3Normalize(player->camera.up);

    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, up));

    right.y = 0;
    right = Vector3Scale(right, distance);

    player->postition = Vector3Add(player->postition, right);
    player->camera.position = player->postition;
    player->camera.target = (Vector3Add(player->camera.target, right));
}

void move_cam(Player *p) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    Vector3 forward = GetCameraForward(&p->camera);

    CameraYaw(&p->camera, -mouse_pos_delta.x * p->cam_rot_speed * delta_time, false);
    CameraPitch(&p->camera, -mouse_pos_delta.y * p->cam_rot_speed * delta_time, true, false, false);

    /*p->camera.position = p->postition;*/

    if (IsKeyDown(KEY_W)) {
        /*CameraMoveForward(&p->camera, p->move_speed * delta_time, true);*/
        player_move_forward(p, p->move_speed * delta_time);
    }

    if (IsKeyDown(KEY_A)) {
        /*CameraMoveRight(&p->camera, -p->move_speed * delta_time, true);*/
        player_move_right(p, -p->move_speed * delta_time);

        if (p->turn_A == false) {
            p->camera.up = Vector3RotateByAxisAngle(p->camera.up, forward, p->cam_rol_scale);
        }

        p->turn_A = true;
    }

    if (IsKeyReleased(KEY_A)) {
        p->camera.up = Vector3Zero();
        p->camera.up.y = 1.0f;
        p->turn_A = false;
    }

    if (IsKeyDown(KEY_S)) {
        /*CameraMoveForward(&p->camera, -p->move_speed * delta_time, true);*/
        player_move_forward(p, -p->move_speed * delta_time);
    }

    if (IsKeyDown(KEY_D)) {
        /*CameraMoveRight(&p->camera, p->move_speed * delta_time, true);*/
        player_move_right(p, p->move_speed * delta_time);

        if (p->turn_D == false) {
            p->camera.up = Vector3RotateByAxisAngle(p->camera.up, forward, -p->cam_rol_scale);
        }
        p->turn_D = true;
    }
    if (IsKeyReleased(KEY_D)) {
        p->camera.up = Vector3Zero();
        p->camera.up.y = 1.0f;
        p->turn_D = false;
    }
}

void update_player(Player *player) {

    player->postition.y += player->gravity;
}

bool check_colision_test(Player *player, Model floor, Vector3 floor_pos) {

    Vector3 entity_pos = player->postition;
    Vector3 entity_size = player->bounding_box_size;

    Vector3 negative = {entity_pos.x - entity_size.x / 2,
                        entity_pos.y - entity_size.y / 2,
                        entity_pos.z - entity_size.z / 2};

    Vector3 positive = {entity_pos.x + entity_size.x / 2,
                        entity_pos.y + entity_size.y / 2,
                        entity_pos.x + entity_size.z / 2};

    BoundingBox entity_box = (BoundingBox){negative, positive};

    Vector3 floor_size = (Vector3){600, 10, 600};

    Vector3 neg = {floor_pos.x - floor_size.x / 2,
                   floor_pos.y - floor_size.y / 2,
                   floor_pos.z - floor_size.z / 2};

    Vector3 pos = {floor_pos.x + floor_size.x / 2,
                   floor_pos.y + floor_size.y / 2,
                   floor_pos.z + floor_size.z / 2};

    BoundingBox floor_box = (BoundingBox){neg, pos};

    if (CheckCollisionBoxes(entity_box, floor_box)) {

        DrawBoundingBox(floor_box, GREEN);
        DrawBoundingBox(entity_box, GREEN);

        return true;
    }

    return false;
}
