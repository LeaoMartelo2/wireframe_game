#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "models.h"
#include <math.h>

#define SHOTGUN_MODEL "models/low_poly_shotgun/shotgun.gltf"

void init_player(Player *player) {

    player->camera = (Camera3D){0};
    player->camera.position = (Vector3){0, 0, 0};
    player->camera.target = Vector3Zero();
    player->camera.up = (Vector3){0, 1, 0};
    player->camera.fovy = 90.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;
    player->postition = (Vector3){10, 50, 10};
    player->turn_A = false;
    player->turn_D = false;
    player->move_speed = 500;
    player->cam_rot_speed = 0.1f;
    player->cam_rol_scale = 0.01f;
    player->gravity = -150.0f;
    /*player->gravity = 0;*/
    player->bounding_box_size = (Vector3){5, 15, 5};
    player->velocity = Vector3Zero();
    player->is_grounded = false;
    player->viewmodel_pos = Vector3Zero();
    player->viewmodel = LoadModel(SHOTGUN_MODEL);
}

bool check_colision_test(Player *player, bool ground) {

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

            /*DrawBoundingBox(other_bounding_box, GREEN);*/
            /*DrawBoundingBox(player_bounding_box, GREEN);*/

            return true;
        }
    }

    return false;
}

void set_camera_pos(Player *player) {
    player->camera.position = player->postition;
    player->camera.position.y = player->postition.y + 5;
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

void move_player(Player *p) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    float decay_rate = 0.1f;

    /*printf("velocities: {%f, %f}\n", p->forward_velocity, p->sideways_velocity);*/

    update_velocity(&p->velocity.x, decay_rate);
    update_velocity(&p->velocity.z, decay_rate);

    clamp_float(&p->velocity.x);
    clamp_float(&p->velocity.z);

    Vector3 forward = GetCameraForward(&p->camera);

    CameraYaw(&p->camera, -mouse_pos_delta.x * p->cam_rot_speed * delta_time, false);
    CameraPitch(&p->camera, -mouse_pos_delta.y * p->cam_rot_speed * delta_time, true, false, false);

    player_move_forward(p, -p->move_speed * p->velocity.x * delta_time);
    player_move_right(p, -p->move_speed * p->velocity.z * delta_time);

    if (check_colision_test(p, false)) {

        /* push in the oposite direction */
        player_move_forward(p, +p->move_speed * p->velocity.x * delta_time);
        player_move_right(p, +p->move_speed * p->velocity.z * delta_time);

        return;
    }

    if (IsKeyDown(KEY_W)) {
        p->velocity.x -= p->move_speed * delta_time;
    }

    if (IsKeyDown(KEY_A)) {
        p->velocity.z += p->move_speed * delta_time;

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
        p->velocity.x += p->move_speed * delta_time;
    }

    if (IsKeyDown(KEY_D)) {
        p->velocity.z -= p->move_speed * delta_time;

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

    temp.y += 4.5f;

    if (player->velocity.x != 0.0f) {

        temp.y += sinf(GetTime()) * 0.2;
    } else {
        temp.y += sinf(GetTime()) * 0.1;
    }

    player->viewmodel_pos = temp;
}

void draw_viewmodel(Player *player) {

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

void update_player(Player *player) {

    /*Sound reload = LoadSound("sounds/reload.wav");*/

    float delta_time = GetFrameTime();

    update_viewmodel_pos(player);

    update_gravity(&player->velocity.y, player->gravity, 15.0f);

    if (check_colision_test(player, true)) {
        player->is_grounded = true;

    } else {
        player_move_vertical(player, player->velocity.y * delta_time);
        update_viewmodel_pos(player);
        player->is_grounded = false;
    }

    if (IsKeyDown(KEY_SPACE)) {
        if (player->is_grounded) {
            player->velocity.y = 300;

            player_move_vertical(player, player->velocity.y * delta_time);
            update_viewmodel_pos(player);
            player->is_grounded = false;
        }
    }

    if (IsKeyDown(KEY_F)) {
        player->faceup = true;
        /*PlaySound(reload);*/
    } else {
        player->faceup = false;
    }
}
