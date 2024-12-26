#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include <math.h>

BoundingBox player_calculate_boundingbox(Player *player);

void player_setup(Player *player) {
    player->camera = (Camera3D){0};
    player->camera.position = Vector3Zero();
    player->camera.target = Vector3Zero();
    player->camera.up = (Vector3){0, 1, 0};
    player->camera.fovy = 90.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;
    player->camera_misc.camera_tilt = 0.01f;
    player->camera_misc.mouse_sens = 0.1f;

    player->pos = (Vector3){50, 50, 50};
    player->move_speed = 500.0f;
    player->velocity = Vector3Zero();
    player->acc_rate = 0.05f;
    player->gravity = -150.0f;
    player->is_grounded = false;

    player->input.forwards = 0.0f;
    player->input.sideways = 0.0f;
    player->input.up_down = 0.0f;

    player->collision.bounding_box_size = (Vector3){5, 15, 5};
    player->collision.bounding_box = player_calculate_boundingbox(player);
}

BoundingBox player_calculate_boundingbox(Player *player) {

    Vector3 entity_size = player->collision.bounding_box_size;

    Vector3 negative = {player->pos.x - entity_size.x / 2,
                        player->pos.y - entity_size.y / 2,
                        player->pos.z - entity_size.z / 2};

    Vector3 positive = {player->pos.x + entity_size.x / 2,
                        player->pos.y + entity_size.y / 2,
                        player->pos.z + entity_size.z / 2};

    BoundingBox player_bounding_box = (BoundingBox){negative, positive};

    return player_bounding_box;
}

/*bool check_colision_test(Player *player, bool ground) {

    Vector3 other_pos = {0};
    Vector3 other_size = {0};

    for (int i = 0; i < player->geometry_count; i++) {

        if (ground == false) {
            other_pos = player->geometry[i].pos;
            other_size = player->geometry[i].size;
        }

        if (ground == true) {
            other_pos = player->ground_geometry[i].pos;
            other_size = player->ground_geometry[i].size;
        }

        Vector3 entity_pos = player->postition;
        Vector3 entity_size = player->bounding_box_size;

        Vector3 negative = {entity_pos.x - entity_size.x / 2,
                            entity_pos.y - entity_size.y / 2,
                            entity_pos.z - entity_size.z / 2};

        Vector3 positive = {entity_pos.x + entity_size.x / 2,
                            entity_pos.y + entity_size.y / 2,
                            entity_pos.z + entity_size.z / 2};

        BoundingBox player_bounding_box = (BoundingBox){negative, positive};

        player->bounding_box = player_bounding_box;

        Vector3 negative_other = {other_pos.x - other_size.x / 2,
                                  other_pos.y - other_size.y / 2,
                                  other_pos.z - other_size.z / 2};

        Vector3 positive_other = {other_pos.x + other_size.x / 2,
                                  other_pos.y + other_size.y / 2,
                                  other_pos.z + other_size.z / 2};

        BoundingBox other_bounding_box = (BoundingBox){negative_other, positive_other};

        if (CheckCollisionBoxes(player_bounding_box, other_bounding_box)) {

            //DrawBoundingBox(other_bounding_box, GREEN);
            //DrawBoundingBox(player_bounding_box, GREEN);

            return true;
        }
    }

    return false;
} */

void player_update_camera(Player *player) {
    player->camera.position = player->pos;
    player->camera.position.y = player->pos.y + 5;
    player->camera.up.y = 1.0f;
    player->camera.target = player->camera.target;
}

Vector3 player_get_forward(Player *player) {

    return Vector3Normalize(Vector3Subtract(player->camera.target, player->camera.position));
}

Vector3 player_get_up(Player *player) {

    return Vector3Normalize(player->camera.up);
}

Vector3 player_get_right(Player *player) {
    Vector3 forward = player_get_forward(player);
    Vector3 up = player_get_up(player);

    return Vector3Normalize(Vector3CrossProduct(forward, up));
}

void player_move_forward(Player *player, float distance) {

    Vector3 forward = player_get_forward(player);

    /* project vector in to world plane */
    forward.y = 0;
    forward = Vector3Normalize(forward);

    /* scale by distance */
    forward = Vector3Scale(forward, distance);

    player->pos = Vector3Add(player->pos, forward);
    player_update_camera(player);
    // keep target forwards to player pos
    player->camera.target = Vector3Add(player->camera.target, forward);
}

void player_move_right(Player *player, float distance) {

    Vector3 right = player_get_right(player);

    right.y = 0;
    right = Vector3Scale(right, distance);

    player->pos = Vector3Add(player->pos, right);
    player_update_camera(player);
    // move target along
    player->camera.target = (Vector3Add(player->camera.target, right));
}

void player_move_vertical(Player *player, float distance) {

    Vector3 up = player_get_up(player);

    up = Vector3Scale(up, distance);

    player->pos = Vector3Add(player->pos, up);
    player_update_camera(player);
    // move camera along
    player->camera.target = Vector3Add(player->camera.target, up);
}

void player_velocity_decay(float *velocity, float decay_rate) {

    if (*velocity > 0.0f) {
        *velocity -= decay_rate;
    }

    if (*velocity < 0.0f) {
        *velocity += decay_rate;
    }
}

void player_update_gravity(float *velocity, float gravity, float rate) {

    if (*velocity > gravity) {
        *velocity -= rate;
    }
}

void player_get_input(Player *player) {

    if (IsKeyDown(KEY_W)) {
        float forwards = player->input.forwards;
        forwards += player->acc_rate;
        forwards = Clamp(forwards, -1.0f, 1.0f);
        player->input.forwards = forwards;
    }

    if (IsKeyDown(KEY_S)) {
        float backwards = player->input.forwards;
        backwards -= player->acc_rate;
        backwards = Clamp(backwards, -1.0f, 1.0f);
        player->input.forwards = backwards;
    }

    if (IsKeyDown(KEY_A)) {
        float left = player->input.sideways;
        left += player->acc_rate;
        left = Clamp(left, -1.0f, 1.0f);
        player->input.sideways = left;
    }

    if (IsKeyDown(KEY_D)) {
        float right = player->input.sideways;
        right -= player->acc_rate;
        right = Clamp(right, -1.0f, 1.0f);
        player->input.sideways = right;
    }
}

void move_player(Player *player) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    float decay_rate = 0.1f;

    player_velocity_decay(&player->velocity.x, decay_rate);
    player_velocity_decay(&player->velocity.z, decay_rate);

    Vector3 forward = GetCameraForward(&player->camera);

    CameraYaw(&player->camera, -mouse_pos_delta.x * player->camera_misc.mouse_sens * delta_time, false);
    CameraPitch(&player->camera, -mouse_pos_delta.y * player->camera_misc.mouse_sens * delta_time,
                true, false, false);

    player_get_input(player);

    player_move_forward(player, player->move_speed * player->input.forwards * delta_time);
    // if the sideways input is negative, it should move to the left
    player_move_right(player, player->move_speed * player->input.sideways * delta_time);
}

/*void update_viewmodel_pos(Player *player) {

    Vector3 temp = player->viewmodel_pos;

    temp = get_player_forward(player);

    Vector3 right = get_player_right(player);

    temp = Vector3Add(temp, player->postition);
    temp = Vector3Add(temp, right);

    temp.y += 4.5f;

    if (player->velocity.x != 0.0f) {

        temp.y += sinf(GetTime()) * 0.2;
    } else {
        temp.y += sinf(GetTime()) * 0.1;
    }

    player->viewmodel_pos = temp;
} */

/*void draw_viewmodel(Player *player) {

    rlPushMatrix();
    rlTranslatef(player->viewmodel_pos.x, player->viewmodel_pos.y, player->viewmodel_pos.z);

    Vector3 direction = Vector3Subtract(player->camera.position, player->viewmodel_pos);
    direction = Vector3Normalize(direction);

    float yaw = atan2f(direction.x, direction.z);

    Matrix rotation = MatrixRotateY(yaw + 3.5f + Normalize(player->velocity.z, -50, 50));

    if (player->faceup) {
        Matrix pitch_rotation = MatrixRotateX(yaw + 360);
        rotation = MatrixMultiply(rotation, pitch_rotation);
    }

    rlMultMatrixf(MatrixToFloat(rotation));
    rlScalef(1, 1, 1);
    DrawModel(player->viewmodel, Vector3Zero(), 1, GetColor(0x181818FF));
    DrawModelWires(player->viewmodel, Vector3Zero(), 1, WHITE);
    rlPopMatrix();
}
*/

void update_player(Player *player) {

    float delta_time = GetFrameTime();

    move_player(player);
}
