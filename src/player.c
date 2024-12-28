#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "geometry.h"
#include <math.h>
#include <string.h>

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

    player->pos = (Vector3){150, 7.5f, 50}; // player is 15 units tall, 7.5 makes the feet touch Y = 0
    player->move_speed = 200.0f;
    player->acc_rate = 0.15f;
    player->gravity = -150.0f;
    player->is_grounded = false;

    player->input.forwards = 0.0f;
    player->input.sideways = 0.0f;
    player->input.up_down = 0.0f;

    player->velocity.forwards = 0.0f;
    player->velocity.sideways = 0.0f;

    player->collision.bounding_box_size = (Vector3){5, 15, 5};
    player->collision.bounding_box = player_calculate_boundingbox(player);

    player->viewmodel.model = LoadModel("models/low_poly_shotgun/shotgun.gltf"); // find better solution later
    player->viewmodel.viewmodel_pos = Vector3Zero();

    player->misc.noclip = false;
    player->misc.show_debug = false;
}

void player_set_collision_map(Player *player, Geometry_Array *map_geometry) {
    player->collision.map_geometry = *map_geometry;
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

bool player_checkcollision_geometry(Player *player) {

    for (size_t i = 0; i < player->collision.map_geometry.size; i++) {

        Vector3 player_pos = player->pos;

        BoundingBox player_bounding_box = player->collision.bounding_box;

        /*BoundingBox geometry_bbx = GetModelBoundingBox(player->collision.map_geometry.data[i].model);*/

        Vector3 other_pos = player->collision.map_geometry.data[i].pos;

        Vector3 other_size = player->collision.map_geometry.data[i].size;

        Vector3 negative_other = {other_pos.x - other_size.x / 2,
                                  other_pos.y - other_size.y / 2,
                                  other_pos.z - other_size.z / 2};

        Vector3 positive_other = {other_pos.x + other_size.x / 2,
                                  other_pos.y + other_size.y / 2,
                                  other_pos.z + other_size.z / 2};

        BoundingBox geometry_bbx = (BoundingBox){negative_other, positive_other};

        if (CheckCollisionBoxes(player_bounding_box, geometry_bbx)) {

            return true;
        }
    }

    return false;
}

void player_update_camera(Player *player) {
    player->camera.position = player->pos;
    player->camera.position.y = player->pos.y + 7;
    player->camera.up.y = 1.0f;
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

    float dead_zone = 0.1f;

    float decay_rate = 0.05f;

    // INPUT DECAY TO 0;
    if (player->input.forwards > 0.000000f) {
        player->input.forwards -= decay_rate;
        player->input.forwards = Clamp(player->input.forwards, -1.0f, 1.0f);
    }

    if (player->input.forwards < 0.000000f) {
        player->input.forwards += decay_rate;
        player->input.forwards = Clamp(player->input.forwards, -1.0f, 1.0f);
    }

    if (player->input.sideways > 0.000000f) {
        player->input.sideways -= decay_rate;
        player->input.sideways = Clamp(player->input.sideways, -1.0f, 1.0f);
    }

    if (player->input.sideways < 0.000000f) {
        player->input.sideways += decay_rate;
        player->input.sideways = Clamp(player->input.sideways, -1.0f, 1.0f);
    }

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

    if (IsKeyDown(KEY_SPACE)) {
        player->input.up_down = 1.0f;
    }
    if (IsKeyReleased(KEY_SPACE)) {
        player->input.up_down = 0.0f;
    }

    if (fabs(player->input.forwards) < dead_zone) {
        player->input.forwards = 0.0f;
    }

    if (fabs(player->input.sideways) < dead_zone) {
        player->input.sideways = 0.0f;
    }
}

void player_calculate_velocity(Player *player) {

    float max_speed = player->move_speed;

    player->velocity.forwards = player->move_speed * player->input.forwards;
    player->velocity.forwards = Clamp(player->velocity.forwards, -max_speed, max_speed);
    // x = 500 * (-1 .. 1) (percentage of total)

    player->velocity.sideways = player->move_speed * player->input.sideways;
    player->velocity.sideways = Clamp(player->velocity.sideways, -max_speed, max_speed);
}

void player_move(Player *player) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    CameraYaw(&player->camera, -mouse_pos_delta.x * player->camera_misc.mouse_sens * delta_time, false);
    CameraPitch(&player->camera, -mouse_pos_delta.y * player->camera_misc.mouse_sens * delta_time,
                true, false, false);

    player_get_input(player);

    Player player_copy = *player;

    player_get_input(&player_copy);
    player_calculate_velocity(&player_copy);
    player_move_forward(&player_copy, player_copy.velocity.forwards * delta_time);
    player_move_right(&player_copy, -player_copy.velocity.sideways * delta_time);

    player_copy.collision.bounding_box = player_calculate_boundingbox(&player_copy);
    DrawBoundingBox(player_copy.collision.bounding_box, ORANGE);

    if (player_checkcollision_geometry(&player_copy)) {
        player->velocity.forwards = 0.0f;
        player->velocity.sideways = 0.0f;
        player_copy.velocity.forwards = 0.0f;
        player_copy.velocity.sideways = 0.0f;
        player_copy.pos = player->pos;
        return;
    }

    player_get_input(player);
    player_calculate_velocity(player);

    player_move_forward(player, player->velocity.forwards * delta_time);
    // if the sideways input is negative, it should move to the left
    player_move_right(player, -player->velocity.sideways * delta_time);

    player->collision.bounding_box = player_calculate_boundingbox(player);
}

void player_update_viewmodel(Player *player) {

    Vector3 temp = player->viewmodel.viewmodel_pos;

    temp = player_get_forward(player);

    Vector3 right = player_get_right(player);

    temp = Vector3Add(temp, player->pos);
    temp = Vector3Add(temp, right);

    temp.y = player->pos.y + 6.5f;

    player->viewmodel.viewmodel_pos = temp;
}

void player_draw_viewmodel(Player *player) {

    rlPushMatrix();
    rlTranslatef(player->viewmodel.viewmodel_pos.x,
                 player->viewmodel.viewmodel_pos.y,
                 player->viewmodel.viewmodel_pos.z);

    Vector3 direction = Vector3Subtract(player->camera.position, player->viewmodel.viewmodel_pos);
    direction = Vector3Normalize(direction);

    float yaw = atan2f(direction.x, direction.z);

    Matrix rotation = MatrixRotateY(yaw + 3.5f + Normalize(player->input.sideways, -1.5, 1.5));

    rlMultMatrixf(MatrixToFloat(rotation));
    rlScalef(1, 1, 1);
    DrawModel(player->viewmodel.model, Vector3Zero(), 1, GetColor(0x181818FF));
    DrawModelWires(player->viewmodel.model, Vector3Zero(), 1, WHITE);
    rlPopMatrix();
}

void player_update(Player *player) {

    player_move(player);
    player_update_camera(player);
    player_update_viewmodel(player);

    if (IsKeyPressed(KEY_F3)) {
        player->misc.show_debug = !player->misc.show_debug;
    }
}

void player_debug(Player *player) {

    if (!player->misc.show_debug) {
        return;
    }

    if (player_checkcollision_geometry(player)) {
        DrawText("TRUE", GetScreenWidth() / 2, GetScreenHeight() / 2, 50, GREEN);
    }

    DrawText(TextFormat("Position:\nX: %.2f, Y: %.2f, Z: %.2f\n"
                        "Input:\n -> Forward: %f\n -> Sideways: %f\n -> Upwards: %f\n"
                        "Velocity:\n -> Forward: %.2f\n -> Sideways: %.2f\n",
                        player->pos.x, player->pos.y, player->pos.z,
                        player->input.forwards, player->input.sideways, player->input.up_down,
                        player->velocity.forwards, player->velocity.sideways),
             10, 10,
             20, WHITE);
}

void player_debug3D(Player *player) {

    if (!player->misc.show_debug) {
        return;
    }

    DrawBoundingBox(player->collision.bounding_box, GREEN);
}

void player_draw3D(Player *player) {

    player_draw_viewmodel(player);

    player_debug3D(player);
}

void player_draw_hud(Player *player) {

    player_debug(player);
}
