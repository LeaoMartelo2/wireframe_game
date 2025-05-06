#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "collision.h"
#include "include/lognest.h"
#include "misc.h"
#include <math.h>

#define FILL_COLOR GetColor(0x181818FF)

#define VIEWMODEL_PATH "assets/models/low_poly_shotgun/shotgun.gltf"

Player::Player() {

    lognest_trace("[Player] Loading Player data.");

    camera = {0};
    camera.position = Vector3Zero();
    camera.target = Vector3Zero();
    camera.up = {0, 1, 0};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera_misc.camera_tilt = 0.0f;
    camera_misc.mouse_sens = 0.1f;

    lognest_debug(" ┗>[Player] Camera created.");

    move_speed = 150.0f;
    side_speed = 120.0f;
    jump_speed = 28.0f;
    acc_rate = 0.15f;
    gravity = 40.0f;
    is_grounded = false;

    velocity = {0, 0, 0};

    input.forwards = 0.0f;
    input.sideways = 0.0f;
    input.up_down = 0.0f;

    lognest_debug(" ┗>[Player] Calculated movement values.");

    collider.size = {5, 15, 5};

    collider.populate();

    lognest_debug(" ┗>[Player] Player collision calculated.");

    if (file_exists(VIEWMODEL_PATH)) {
        viewmodel.model = LoadModel(VIEWMODEL_PATH);
        lognest_debug(" ┗>[Player] Viewmodel loaded from '%s'.", VIEWMODEL_PATH);
    } else {
        lognest_error("[Player] Viewmodel could not be loaded from '%s'.", VIEWMODEL_PATH);
        exit(EXIT_FAILURE);
    }

    viewmodel.viewmodel_pos = Vector3Zero();

    gameplay.health = 250;
    gameplay.ammo = 20;

    misc.show_debug = false;
    misc.noclip = false;
    misc.no_gravity = false;

    lognest_debug(" ┗>[Player] Player debug-tools loaded.");

    lognest_trace("[Player] Player loaded.");
}

// Modified implementation of CameraYaw from rcamera.h
// https://github.com/raysan5/raylib
void Player::camera_yaw(float angle) {

    Vector3 up = GetCameraUp(&camera);

    Vector3 target_pos = Vector3Subtract(camera.target, camera.position);
    target_pos = Vector3RotateByAxisAngle(target_pos, up, angle);

    camera.target = Vector3Add(camera.position, target_pos);
}

// Modified implementation of CameraPitch from rcamera.h
// https://github.com/raysan5/raylib
void Player::camera_pitch(float angle) {

    Vector3 up = GetCameraUp(&camera);

    Vector3 target_pos = Vector3Subtract(camera.target, camera.position);

    float max_ang_up = Vector3Angle(up, target_pos);
    max_ang_up -= 0.15f;
    if (angle > max_ang_up) {
        angle = max_ang_up;
    }

    float max_ang_down = Vector3Angle(Vector3Negate(up), target_pos);
    max_ang_down *= -1.0f;
    max_ang_down += 0.15f;
    if (angle < max_ang_down) {
        angle = max_ang_down;
    }

    Vector3 right = GetCameraRight(&camera);

    target_pos = Vector3RotateByAxisAngle(target_pos, right, angle);

    camera.target = Vector3Add(camera.position, target_pos);
}

void Player::update_camera() {
    camera.position = collider.pos;
    camera.position.y = collider.pos.y + 7;
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

void Player::update_gravity() {

    if (misc.no_gravity) {
        velocity.y = 0;
        return;
    }

    float delta_time = GetFrameTime();

    velocity.y -= gravity * delta_time;

    collider.pos = Vector3Add(collider.pos, Vector3Scale(velocity, delta_time));
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

void Player::jump() {

    if (is_grounded) {
        velocity.y = jump_speed;
        is_grounded = false;
    }
}

void Player::move(const std::vector<Collider> &map_colliders) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    camera_yaw(-mouse_pos_delta.x * camera_misc.mouse_sens * delta_time);
    camera_pitch(-mouse_pos_delta.y * camera_misc.mouse_sens * delta_time);

    if (IsKeyDown(KEY_SPACE)) {
        jump();
    }

    Vector3 movement = {0, 0, 0};

    Vector3 forward = Vector3Normalize(get_forward());
    Vector3 right = Vector3Normalize(get_right());

    // allow player to go vertical looking up and down if noclipping
    if (!misc.noclip) {
        right.y = 0;
        forward.y = 0;
    }

    get_input();

    movement += forward * move_speed * input.forwards * delta_time;
    movement += right * side_speed * input.sideways * -1 * delta_time; /* sideways speed is inverted */

    // movement += direction * move speed * (-1 ... 1) * delta_time
    //                                        ^ this leaves us with a percentage of total max speed
    // based on how long the player held the directional key
    // as in: slight tap = move slowly; held the key for a bit = fullspeed

    collider.pos = Vector3Add(collider.pos, movement);
    camera.target = Vector3Add(camera.target, Vector3Multiply(get_forward(), {10, 10, 10}));
    // ^ move camera target along with the player

    update_gravity();

    collider.is_colliding = false;

    /* skip collision checking if noclipping */
    if (!misc.noclip) {

        /* check collision */
        for (auto &map_collider : map_colliders) {

            MTV mtv;
            if (collider_check_collision(collider, map_collider, &mtv)) {

                collider.is_colliding = true;

                /* compute translation from collision */
                Vector3 mtv_direction = Vector3Normalize(mtv.axis);
                Vector3 translation = Vector3Scale(mtv_direction, mtv.depth);
                Vector3 to_player = Vector3Subtract(collider.pos, map_collider.pos);

                if (Vector3DotProduct(to_player, mtv_direction) < 0) {
                    translation = Vector3Negate(translation);
                }

                /* then apply it */
                collider.pos = Vector3Add(collider.pos, translation);

                /* check if its vertical from above (set grounded if true) */
                if (mtv.axis.y == 1 && translation.y > 0) {
                    is_grounded = true;
                    velocity.y = 0;
                }
            }
        }
    }

    velocity.x = movement.x;
    velocity.z = movement.z;
}

void Player::update_viewmodel() {

    Vector3 new_pos = get_forward();

    Vector3 right = get_right();

    new_pos = Vector3Add(new_pos, collider.pos);
    new_pos = Vector3Add(new_pos, right);

    new_pos.y = collider.pos.y + 6.5f;

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

void Player::update(const std::vector<Collider> &map_colliders) {

    if (misc.noclip) {
        misc.no_gravity = true;
    }
    if (!misc.noclip) {
        misc.no_gravity = false;
    }

    update_gravity();
    /* don't even question why this has to be called twice */

    move(map_colliders);

    update_viewmodel();
    update_camera();

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

void Player::set_pos(Vector3 new_pos) {
    collider.pos = new_pos;

    lognest_debug(" ┗> [Player] Teleported to %.2f, %.2f, %.2f",
                  new_pos.x, new_pos.y, new_pos.z);
}

void Player::set_looking_at(Vector3 new_pos) {
    camera.target = new_pos;
}

void Player::damage(long ammount) {

    gameplay.health += ammount;
}

void Player::give_ammo(long ammount) {
    gameplay.ammo += ammount;
}

void Player::debug() {
    if (!misc.show_debug) {
        return;
    }

    int width = 6;

    std::string text_dbg = TextFormat("Position:\nX: %.2f, Y: %.2f, Z: %.2f\n"
                                      "Input:\n -> Forward: { %f }\n -> Sideways: { %f }\n"
                                      "Velocity:\n -> { %*.*f, %*.*f, %*.*f }\n"
                                      "is_grounded: %s\n"
                                      "is_colliding: %s",
                                      collider.pos.x, collider.pos.y, collider.pos.z,
                                      input.forwards, input.sideways,
                                      width, 2, velocity.x,
                                      width, 2, velocity.y,
                                      width, 2, velocity.z,
                                      bool_to_string(is_grounded),
                                      bool_to_string(collider.is_colliding));

    DrawText(text_dbg.c_str(), 10, 10, 20, WHITE);

    if (misc.noclip) {
        DrawText("NoClip enabled", GetScreenWidth() - 150, 15, 20, WHITE);
    }

    DrawFPS(GetScreenWidth() - 100, 0);
}

void Player::debug_3d() {
    if (!misc.show_debug) {
        return;
    }
    /* @TODO:
     * fix this not drawing at the player's position
     */
    BoundingBox player_bounding_box = GetModelBoundingBox(collider.model);
    DrawBoundingBox(player_bounding_box, GREEN);
}

void Player::draw() {
    draw_viewmodel();
    debug_3d();
}

void Player::draw_hud() {
    debug();

    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 2, WHITE);

#ifdef DEBUG
    DrawText("Debug build", GetScreenWidth() - 150, GetScreenHeight() - 100, 20, WHITE);
#endif // DEBUG

    /*DrawText(TextFormat("%d", gameplay.health),*/
    /*(GetScreenWidth() / 32), GetScreenHeight() / 2 + 150, 50, GetColor(0xFF0000FF));*/

    /*DrawText(TextFormat("%d", gameplay.ammo),*/
    /*(GetScreenWidth() - 150), GetScreenHeight() / 2 + 150, 50, LIGHTGRAY);*/
}
