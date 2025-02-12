#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "geometry.h"
#include "include/lognest.h"
#include "misc.h"
#include <math.h>
#include <vector>

#define FILL_COLOR GetColor(0x181818FF)

#define VIEWMODEL_PATH "assets/models/low_poly_shotgun/shotgun.gltf"

Model empty_model = {0};

Player::Player() {

    lognest_trace("[Player] Loading Player data.");

    camera = {0};
    camera.position = Vector3Zero();
    camera.target = Vector3Zero();
    camera.up = {0, 1, 0};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera_misc.camera_tilt = 0.01f;
    camera_misc.mouse_sens = 0.1f;

    lognest_debug(" ┗>[Player] Camera created.");

    pos = {150, 20, 50};
    move_speed = 200.0f;
    side_speed = 180.0f;
    air_boost = 1.10f;
    acc_rate = 0.15f;
    gravity = 9.806f;
    max_acell = 250.0f;
    is_grounded = false;

    velocity.forwards = 0;
    velocity.sideways = 0;
    velocity.vertical = 0;

    lognest_debug(" ┗>[Player] Setup default movement variables.");

    input.forwards = 0.0f;
    input.sideways = 0.0f;
    input.up_down = 0.0f;

    collision.bounding_box_size = {5, 15, 5};
    collision.bounding_box = calculate_boundingbox();

    lognest_debug(" ┗>[Player] Player Bounding Box calculated.");

    if (file_exists(VIEWMODEL_PATH)) {
        viewmodel.model = LoadModel(VIEWMODEL_PATH);
        lognest_debug(" ┗>[Player] Viewmodel loaded from '%s'.", VIEWMODEL_PATH);
    } else {
        lognest_error("[Player] Viewmodel could not be loaded from '%s'.", VIEWMODEL_PATH);
        exit(EXIT_FAILURE);
    }

    viewmodel.viewmodel_pos = Vector3Zero();

    misc.show_debug = false;
    misc.noclip = false;
    misc.no_gravity = false;

    lognest_debug(" ┗>[Player] Player debug-tools loaded.");

    lognest_trace("[Player] Player loaded.");
}

Player::Player(const Player &other) {

    pos = other.pos;
    move_speed = other.move_speed;
    acc_rate = other.acc_rate;

    camera.target = other.camera.target;

    camera_misc = other.camera_misc;

    collision.bounding_box_size = {5, 15, 5};
    /*collision.bounding_box = other.collision.bounding_box;*/
    move_speed = other.move_speed;
    acc_rate = other.acc_rate;
    side_speed = other.side_speed;
    air_boost = other.air_boost;
    is_grounded = other.is_grounded;
    velocity.forwards = other.velocity.forwards;
    velocity.sideways = other.velocity.sideways;

    input = other.input;
}

BoundingBox Player::calculate_boundingbox() {

    Vector3 entity_size = collision.bounding_box_size;

    Vector3 negative = {pos.x - entity_size.x / 2,
                        pos.y - entity_size.y / 2,
                        pos.z - entity_size.z / 2};

    Vector3 positive = {pos.x + entity_size.x / 2,
                        pos.y + entity_size.y / 2,
                        pos.z + entity_size.z / 2};

    BoundingBox player_bounding_box = {negative, positive};

    return player_bounding_box;
}

bool Player::check_collision_geometry(std::vector<Geometry> &map_geometry) {

    if (misc.noclip) {
        return false;
    }

    /*for (size_t i = 0; map_geometry.size(); ++i) {*/
    for (auto i : map_geometry) {

        Vector3 other_pos = i.pos;
        Vector3 other_size = i.size;

        Vector3 negative_other = {other_pos.x - other_size.x / 2,
                                  other_pos.y - other_size.y / 2,
                                  other_pos.z - other_size.z / 2};

        Vector3 positive_other = {other_pos.x + other_size.x / 2,
                                  other_pos.y + other_size.y / 2,
                                  other_pos.z + other_size.z / 2};

        BoundingBox geometry_bounding_box = {negative_other, positive_other};

        if (CheckCollisionBoxes(collision.bounding_box, geometry_bounding_box)) {
            return true;
        }
    }

    return false;
}

bool Player::check_collision_floor(std::vector<Floor> &map_floor) {

    if (misc.noclip) {
        return false;
    }

    for (auto i : map_floor) {
        Vector3 other_pos = i.pos;
        Vector3 other_size = i.size;

        Vector3 negative_other = {other_pos.x - other_size.x / 2,
                                  other_pos.y - other_size.y / 2,
                                  other_pos.z - other_size.z / 2};

        Vector3 positive_other = {other_pos.x + other_size.x / 2,
                                  other_pos.y + other_size.y / 2,
                                  other_pos.z + other_size.z / 2};

        BoundingBox floor_bounding_box = {negative_other, positive_other};

        if (CheckCollisionBoxes(collision.bounding_box, floor_bounding_box)) {
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

Vector3 Player::move_forward(float distance) {

    Vector3 forward = get_forward();

    forward.y = 0;
    forward = Vector3Normalize(forward);

    forward = Vector3Scale(forward, distance);

    /*update_camera();*/
    /*camera.target = Vector3Add(camera.target, forward);*/

    return forward;
}

Vector3 Player::move_right(float distance) {

    Vector3 right = get_right();

    right.y = 0;
    right = Vector3Scale(right, distance);

    /*update_camera();*/
    /*camera.target = Vector3Add(camera.target, right);*/

    return right;
}

Vector3 Player::move_vertical(float distance) {

    Vector3 up = get_up();

    up = Vector3Scale(up, distance);

    /*pos = Vector3Add(pos, up);*/
    /*update_camera();*/

    /*camera.target = Vector3Add(camera.target, up);*/

    return up;
}

Vector3 Player::new_pos(float distance_foward, float distance_right) {

    Vector3 forward = move_forward(distance_foward);
    Vector3 right = move_right(distance_right);

    return Vector3Add(forward, right);
}

void Player::update_gravity() {

    if (misc.no_gravity) {
        velocity.vertical = 0;
        return;
    }

    if (!is_grounded) {
        if (velocity.vertical >= -max_acell) {
            velocity.vertical -= gravity;
        } else {
            velocity.vertical = -max_acell;
        }
    }
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

    if (input.up_down) {
        input.up_down = 0;
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

    if (IsKeyDown(KEY_SPACE)) {
        if (is_grounded) {
            input.up_down = 1.0f;
            is_grounded = false;
        }
    }

    if (fabs(input.forwards) < dead_zone) {
        input.forwards = 0.0f;
    }

    if (fabs(input.sideways) < dead_zone) {
        input.sideways = 0.0f;
    }
}

void Player::calculate_velocity() {

    static float scale = 1.0f;

    scale = is_grounded ? 1.0f : air_boost;

    velocity.forwards = move_speed * input.forwards * scale;
    velocity.forwards = Clamp(velocity.forwards, -move_speed * scale, move_speed * scale);

    // x = move_speed * (-1 ... 1) <-- this leaves us with a percentage of total max speed
    // based on how long the player held the directional key
    // as in: slight tap = move slowly; held the key for a bit = fullspeed
    // makes movement more natural to controll

    velocity.sideways = side_speed * input.sideways * scale;
    velocity.sideways = Clamp(velocity.sideways, -side_speed * scale, side_speed * scale);
}

void Player::move(std::vector<Geometry> &map_geometry, std::vector<Floor> &map_floor) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    CameraYaw(&camera, -mouse_pos_delta.x * camera_misc.mouse_sens * delta_time, false);
    CameraPitch(&camera, -mouse_pos_delta.y * camera_misc.mouse_sens * delta_time,
                true, false, false);

    get_input();

    collision.bounding_box = calculate_boundingbox();
    calculate_velocity();
    Vector3 move_a = move_forward(velocity.forwards * delta_time) + pos;
    Vector3 move_b = move_right(-velocity.sideways * delta_time) + pos;
    Vector3 move_c = new_pos(velocity.forwards * delta_time, -velocity.sideways * delta_time) + pos;

    /*lognest_error("%.2f", move_c.x);*/

    DrawLine3D(pos, move_a, GREEN);
    DrawLine3D(pos, move_b, BLUE);
    DrawLine3D(pos, move_c, RED);

    if (check_collision_floor(map_floor)) {
        is_grounded = true;
    } else {

        move_vertical(velocity.vertical * delta_time);
        is_grounded = false;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        if (is_grounded) {
            velocity.vertical = 150;

            move_vertical(velocity.vertical * delta_time);
            is_grounded = false;
        }
    }

    if (misc.noclip) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            move_vertical(-100 * delta_time);
        }

        if (IsKeyDown(KEY_SPACE)) {
            move_vertical(100 * delta_time);
        }
    }

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

    /*DrawCube(camera.target, 5, 5, 5, ORANGE);*/
}

void Player::update(std::vector<Geometry> &map_geometry, std::vector<Floor> &map_floor) {

    if (misc.noclip) {
        misc.no_gravity = true;
    }
    if (!misc.noclip) {
        misc.no_gravity = false;
    }

    update_gravity();
    move(map_geometry, map_floor);
    update_camera();
    update_viewmodel();

    if (IsKeyPressed(KEY_F3)) {
        misc.show_debug = !misc.show_debug;
        lognest_debug("[Player] Toggled debug menu '%s' -> '%s'.", bool_to_string(!misc.show_debug),
                      bool_to_string(misc.show_debug));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsKeyPressed(KEY_N)) {
            misc.noclip = !misc.noclip;
            lognest_debug("[Player] Toggle NoClip '%s' -> '%s'",
                          bool_to_string(!misc.noclip), bool_to_string(misc.noclip));
        }
    }
}

void Player::debug() {
    if (!misc.show_debug) {
        return;
    }

    DrawText(TextFormat("Position:\nX: %.2f, Y: %.2f, Z: %.2f\n"
                        "Input:\n -> Forward: %f\n -> Sideways: %f\n"
                        "Velocity:\n -> Forward: %.2f\n -> Vertical: %.2f\n -> Sideways: %.2f\n"
                        "Grounded: %s\n",
                        pos.x, pos.y, pos.z,
                        input.forwards, input.sideways,
                        velocity.forwards, velocity.vertical, velocity.sideways,
                        bool_to_string(is_grounded)),
             10, 10,
             20, WHITE);
    if (misc.noclip) {
        DrawText("NoClip enabled", GetScreenWidth() - 150, 15, 20, WHITE);
    }

    DrawFPS(GetScreenWidth() - 100, 0);
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

    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 2, WHITE);

    // health /  ammo /  etc
}
