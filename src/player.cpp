#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "geometry.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#define FILL_COLOR GetColor(0x181818FF)

#define VIEWMODEL_PATH "models/low_poly_shotgun/shotgun.gltf"
Player::Player() {
    camera = *new Camera3D;
    camera.position = Vector3Zero();
    camera.target = Vector3Zero();
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera_misc.camera_tilt = 0.01f;
    camera_misc.mouse_sens = 0.1f;

    pos = (Vector3){150, 7.5, 50};
    move_speed = 200.0f;
    acc_rate = 0.15f;
    gravity = -10.0f;
    is_grounded = false;

    input.forwards = 0.0f;
    input.sideways = 0.0f;
    input.up_down = 0.0f;

    collision.bounding_box_size = (Vector3){5, 15, 5};
    collision.bounding_box = calculate_boundingbox();

    viewmodel.model = LoadModel("models/low_poly_shotgun/shotgun.gltf"); // find better solution later
    viewmodel.viewmodel_pos = Vector3Zero();

    misc.show_debug = true;
    misc.noclip = false;
    misc.no_gravity = true;
}

BoundingBox Player::calculate_boundingbox() {

    Vector3 entity_size = collision.bounding_box_size;

    Vector3 negative = {pos.x - entity_size.x / 2,
                        pos.y - entity_size.y / 2,
                        pos.z - entity_size.z / 2};

    Vector3 positive = {pos.x + entity_size.x / 2,
                        pos.y + entity_size.y / 2,
                        pos.z + entity_size.z / 2};

    BoundingBox player_bounding_box = (BoundingBox){negative, positive};

    return player_bounding_box;
}

bool Player::check_collision_geometry(const std::vector<Geometry> &map_geometry) {

    if (misc.noclip) {
        return false;
    }

    for (int i = 0; map_geometry.size(); i++) {

        Vector3 other_pos = map_geometry[i].pos;
        Vector3 other_size = map_geometry[i].size;

        Vector3 negative_other = {other_pos.x - other_size.x / 2,
                                  other_pos.y - other_size.y / 2,
                                  other_pos.z - other_size.z / 2};

        Vector3 positive_other = {other_pos.x + other_size.x / 2,
                                  other_pos.y + other_size.y / 2,
                                  other_pos.z + other_size.z / 2};

        BoundingBox geometry_bounding_box = (BoundingBox){negative_other, positive_other};

        if (CheckCollisionBoxes(collision.bounding_box, geometry_bounding_box)) {
            return true;
        }
    }

    return false;
}

void Player::update_camera() {
    camera.position = pos;
    camera.position.y = pos.y + 7;
    camera.up.y = 1.0f;
}

Vector3 Player::get_forward() {
    return Vector3Normalize(Vector3Subtract(camera.target, camera.position));
}

Vector3 Player::get_up() {
    return Vector3Normalize(camera.up);
}

Vector3 Player::get_right() {
    Vector3 forward = get_forward();
    Vector3 up = get_up();

    return Vector3Normalize(Vector3CrossProduct(forward, up));
}

void Player::move_forward(float distance) {

    Vector3 forward = get_forward();

    forward.y = 0;
    forward = Vector3Normalize(forward);

    forward = Vector3Scale(forward, distance);

    pos = Vector3Add(pos, forward);
    update_camera();
    camera.target = Vector3Add(camera.target, forward);
}

void Player::move_right(float distance) {

    Vector3 right = get_right();

    right.y = 0;
    right = Vector3Scale(right, distance);

    pos = Vector3Add(pos, right);
    update_camera();

    camera.target = Vector3Add(camera.target, right);
}

void Player::move_vertical(float distance) {

    Vector3 up = get_up();

    up = Vector3Scale(up, distance);

    pos = Vector3Add(pos, up);
    update_camera();

    camera.target = Vector3Add(camera.target, up);
}

void Player::get_input() {

    float dead_zone = 0.1f;

    float decay_rate = 0.05f;

    // decay input to 0;

    if (input.forwards > 0.000000f) {
        input.forwards -= decay_rate;
        input.forwards = Clamp(input.forwards, -1, 1);
    }
    if (input.forwards < 0.000000f) {
        input.forwards += decay_rate;
        input.forwards = Clamp(input.forwards, -1, 1);
    }

    if (input.sideways > 0.000000f) {
        input.sideways -= decay_rate;
        input.forwards = Clamp(input.forwards, -1, 1);
    }
    if (input.sideways < 0.000000f) {
        input.sideways += decay_rate;
        input.forwards = Clamp(input.forwards, -1, 1);
    }

    if (IsKeyDown(KEY_W)) {
        float forwards = input.forwards;
        forwards += acc_rate;
        forwards = Clamp(forwards, -1, 1);
        input.forwards = forwards;
    }
    if (IsKeyDown(KEY_S)) {
        float backwards = input.forwards;
        backwards -= acc_rate;
        backwards = Clamp(backwards, -1, 1);
        input.forwards = backwards;
    }

    if (IsKeyDown(KEY_A)) {
        float left = input.sideways;
        left += acc_rate;
        left = Clamp(left, -1, 1);
        input.sideways = left;
    }
    if (IsKeyDown(KEY_D)) {
        float right = input.sideways;
        right -= acc_rate;
        right = Clamp(right, -1, 1);
        input.sideways = right;
    }

    if (fabs(input.forwards) < dead_zone) {
        input.forwards = 0.0f;
    }

    if (fabs(input.sideways) < dead_zone) {
        input.sideways = 0.0f;
    }
}

void Player::calculate_velocity() {

    float max_speed = this->move_speed;

    velocity.forwards = move_speed * input.forwards;
    velocity.forwards = Clamp(velocity.forwards, -max_speed, max_speed);

    // x = move_speed * (-1 ... 1) <-- this leaves us with a percentage of total max speed
    // based on how long the player held the directional key
    // as in: slight tap = move slowly; held the key for a bit = fullspeed
    // makes movement more natural to controll

    velocity.sideways = move_speed * input.sideways;
    velocity.sideways = Clamp(velocity.sideways, -max_speed, max_speed);
}

void Player::move() {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    CameraYaw(&camera, -mouse_pos_delta.x * camera_misc.mouse_sens * delta_time, false);
    CameraPitch(&camera, -mouse_pos_delta.y * camera_misc.mouse_sens * delta_time,
                true, false, false);

    get_input();

    calculate_velocity();
    move_forward(velocity.forwards * delta_time);
    move_right(-velocity.sideways * delta_time);

    collision.bounding_box = calculate_boundingbox();
}

void Player::update_viewmodel() {

    Vector3 new_pos = get_forward();

    Vector3 right = get_right();

    new_pos = Vector3Add(new_pos, pos);
    new_pos = Vector3Add(new_pos, right);

    new_pos.y = pos.y + 6.5f;

    viewmodel.viewmodel_pos = new_pos;
}

void Player::draw_viewmodel() {

    rlPushMatrix();
    rlTranslatef(viewmodel.viewmodel_pos.x,
                 viewmodel.viewmodel_pos.y,
                 viewmodel.viewmodel_pos.z);

    Vector3 direction = Vector3Subtract(camera.position, viewmodel.viewmodel_pos);
    direction = Vector3Normalize(direction);

    float yaw = atan2f(direction.x, direction.z);

    Matrix rotation = MatrixRotateY(yaw + 3.5f + Normalize(input.sideways, -1.5, 1.5));

    rlMultMatrixf(MatrixToFloat(rotation));
    rlScalef(1, 1, 1);
    DrawModel(viewmodel.model, Vector3Zero(), 1, FILL_COLOR);
    DrawModelWires(viewmodel.model, Vector3Zero(), 1, WHITE);
    rlPopMatrix();
}

void Player::update() {
    move();
    update_camera();
    update_viewmodel();

    if (IsKeyPressed(KEY_F3)) {
        misc.show_debug = !misc.show_debug;
    }
}

void Player::debug() {
    if (!misc.show_debug) {
        return;
    }

    DrawText(TextFormat("Position:\nX: %.2f, Y: %.2f, Z: %.2f\n"
                        "Input:\n -> Forward: %f\n -> Sideways: %f\n -> Upwards: %f\n"
                        "Velocity:\n -> Forward: %.2f\n -> Sideways: %.2f\n",
                        pos.x, pos.y, pos.z,
                        input.forwards, input.sideways, input.up_down,
                        velocity.forwards, velocity.sideways),
             10, 10,
             20, WHITE);
    if (misc.noclip) {
        DrawText("NoClip enabled", GetScreenWidth() - 150, 10, 20, WHITE);
    }
}

void Player::debug_3d() {
    if (!misc.show_debug) {
        return;
    }
    DrawBoundingBox(collision.bounding_box, GREEN);
}

void Player::draw() {

    draw_viewmodel();
    debug_3d();
}

void Player::draw_hud() {
    debug();

    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1.0f, WHITE);

    // health /  ammo /  etc
}
