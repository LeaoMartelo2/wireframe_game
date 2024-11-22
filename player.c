#include "player.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

bool check_colision_test(Player *player, Vector3 floor_pos, Vector3 floor_size) {

    Vector3 entity_pos = player->postition;
    Vector3 entity_size = player->bounding_box_size;

    Vector3 negative = {entity_pos.x - entity_size.x / 2,
                        entity_pos.y - entity_size.y / 2,
                        entity_pos.z - entity_size.z / 2};

    Vector3 positive = {entity_pos.x + entity_size.x / 2,
                        entity_pos.y + entity_size.y / 2,
                        entity_pos.z + entity_size.z / 2};

    BoundingBox entity_box = (BoundingBox){negative, positive};

    /*Vector3 floor_size = (Vector3){600, 10, 600};*/

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

void set_camera_pos(Player *player) {
    player->camera.position = player->postition;
    player->camera.position.y = player->postition.y + 9;
    player->camera.up.y = 1.0f;
    player->camera.target = player->camera.target;
}

Vector3 get_player_forward(Player *player) {

    return Vector3Normalize(Vector3Subtract(player->camera.target, player->camera.position));
}

Vector3 get_player_up(Player *player) {

    return Vector3Normalize(player->camera.up);
}

Vector3 get_player_right(Player *player) {
    Vector3 forward = get_player_forward(player);
    Vector3 up = get_player_up(player);

    return Vector3Normalize(Vector3CrossProduct(forward, up));
}

void player_move_forward(Player *player, float distance) {
    /* direction its facing */

    Vector3 forward = get_player_forward(player);

    /* project vector in to world plane */
    forward.y = 0;
    forward = Vector3Normalize(forward);

    /* scale by distance */
    forward = Vector3Scale(forward, distance);

    player->postition = Vector3Add(player->postition, forward);
    set_camera_pos(player);
    player->camera.target = Vector3Add(player->camera.target, forward);
}

void player_move_right(Player *player, float distance) {

    Vector3 right = get_player_right(player);

    right.y = 0;
    right = Vector3Scale(right, distance);

    player->postition = Vector3Add(player->postition, right);
    set_camera_pos(player);
    player->camera.target = (Vector3Add(player->camera.target, right));
}

void player_move_vertical(Player *player, float distance) {

    Vector3 up = get_player_up(player);

    up = Vector3Scale(up, distance);

    player->postition = Vector3Add(player->postition, up);
    set_camera_pos(player);
    player->camera.target = Vector3Add(player->camera.target, up);
}

void clamp_float(float *velocity) {
    if (*velocity > 1) {
        *velocity = 1;
    }

    if (*velocity < -1) {
        *velocity = -1;
    }

    // deadzone to avoid floating point errors:

    float dead_zone = 0.1;

    if (fabsf(*velocity) < dead_zone) {
        *velocity = 0.0f;
    }
}

void update_velocity(float *velocity, float decay_rate) {

    if (*velocity > 0.0000f) {
        *velocity -= decay_rate;
    }

    if (*velocity < 0.0000f) {
        *velocity += decay_rate;
    }
}

void update_gravity(float *velocity, float gravity, float rate) {

    if (*velocity > gravity) {
        *velocity -= rate;
    }
}

void move_cam(Player *p) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    float decay_rate = 0.1f;

    /*printf("velocities: {%f, %f}\n", p->forward_velocity, p->sideways_velocity);*/

    update_velocity(&p->forward_velocity, decay_rate);
    update_velocity(&p->sideways_velocity, decay_rate);

    clamp_float(&p->forward_velocity);
    clamp_float(&p->sideways_velocity);

    Vector3 forward = GetCameraForward(&p->camera);

    CameraYaw(&p->camera, -mouse_pos_delta.x * p->cam_rot_speed * delta_time, false);
    CameraPitch(&p->camera, -mouse_pos_delta.y * p->cam_rot_speed * delta_time, true, false, false);

    player_move_forward(p, -p->move_speed * p->forward_velocity * delta_time);
    player_move_right(p, -p->move_speed * p->sideways_velocity * delta_time);

    if (IsKeyDown(KEY_W)) {
        p->forward_velocity -= p->move_speed * delta_time;
    }

    if (IsKeyDown(KEY_A)) {

        p->sideways_velocity += 1.0f;

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
        p->forward_velocity += 1.0f;
    }

    if (IsKeyDown(KEY_D)) {

        p->sideways_velocity -= 1.0f;

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

void update_viewmodel_pos(Player *player) {

    Vector3 temp = player->viewmodel_pos;

    temp = get_player_forward(player);

    Vector3 right = get_player_right(player);

    temp = Vector3Add(temp, player->postition);
    temp = Vector3Add(temp, right);

    temp.y += 8.5;

    if (player->forward_velocity != 0.0f) {

        temp.y += sinf(GetTime()) * 0.3;
    } else {
        temp.y += sinf(GetTime()) * 0.2;
    }

    player->viewmodel_pos = temp;
    /*player->viewmodel_rotation = temp.y;*/
}

void draw_viewmodel(Player *player, Model viewmodel) {

    rlPushMatrix();
    rlTranslatef(player->viewmodel_pos.x, player->viewmodel_pos.y, player->viewmodel_pos.z);

    Vector3 direction = Vector3Subtract(player->camera.position, player->viewmodel_pos);
    direction = Vector3Normalize(direction);

    float yaw = atan2f(direction.x, direction.z);

    Matrix rotation = MatrixRotateY(yaw + 3.5f + Normalize(player->sideways_velocity, -10, 10));

    if (player->faceup) {
        Matrix pitch_rotation = MatrixRotateX(yaw + 360);
        rotation = MatrixMultiply(rotation, pitch_rotation);
    }

    rlMultMatrixf(MatrixToFloat(rotation));
    rlScalef(1, 1, 1);
    DrawModelWires(viewmodel, Vector3Zero(), 1, WHITE);
    rlPopMatrix();
}

void update_player(Player *player) {

    float delta_time = GetFrameTime();

    update_viewmodel_pos(player);

    update_gravity(&player->vertical_velocity, player->gravity, 15.0f);

    if (check_colision_test(player, Vector3Zero(), (Vector3){2000, 10, 2000})) {
        player->is_grounded = true;

    } else {
        player_move_vertical(player, player->vertical_velocity * delta_time);
        update_viewmodel_pos(player);
        player->is_grounded = false;
    }

    if (IsKeyDown(KEY_SPACE)) {
        if (player->is_grounded) {
            player->vertical_velocity = 250;

            player_move_vertical(player, player->vertical_velocity * delta_time);
            update_viewmodel_pos(player);
            player->is_grounded = false;
        }
    }

    if (IsKeyDown(KEY_F)) {
        player->faceup = true;
    } else {
        player->faceup = false;
    }
}
