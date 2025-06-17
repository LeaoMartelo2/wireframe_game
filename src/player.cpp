#include "player.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "collision.h"
#include "doors.h"
#include "globals.h"
#include "include/lognest.h"
#include "items.h"
#include "misc.h"
#include <algorithm>
#include <assert.h>
#include <math.h>

#define FILL_COLOR GetColor(0x181818FF)

Player::Player() {

    lognest_trace("[Player] Loading Player data.");

    camera = {};
    camera.position = Vector3Zero();
    camera.target = Vector3Zero();
    camera.up = {0, 1, 0};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera_misc.camera_tilt = 0.0f;
    camera_misc.mouse_sens = 0.1f;

    lognest_debug(" ┗>[Player] Camera created.");

    move_speed = 150.0f;
    side_speed = 85.5f;
    jump_speed = 150.0f;
    max_speed = 5.0;
    acc_rate = 0.15f;
    gravity = 9.5f;
    is_grounded = false;

    velocity = {0, 0, 0};

    input.forwards = 0.0f;
    input.sideways = 0.0f;
    input.up_down = 0.0f;

    lognest_debug(" ┗>[Player] Calculated movement values.");

    collider.size = {5, 15, 5};

    collider.populate();

    lognest_debug(" ┗>[Player] Player collision calculated.");

    clear_inventory();

    inventory.selected_slot = 1;

    current_key = DOORKEY_NONE;

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

    if (is_grounded) {
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

    if (IsKeyPressed(KEY_ONE)) {
        switch_to_slot(1);
    }

    if (IsKeyPressed(KEY_TWO)) {
        switch_to_slot(2);
    }

    if (IsKeyPressed(KEY_THREE)) {
        switch_to_slot(3);
    }
}

void Player::switch_to_slot(size_t slot) {

    if (dynamic_cast<EmptyItem *>(inventory.slot.at(slot))) {
        // empty item
        // @TODO figure out something idk
        return;
    } else {
        /* @TODO: figure out how to prevent the player from
         * switching items before the eqip animation finishes */
        if (slot != inventory.selected_slot) {

            inventory.selected_slot = slot;
            inventory.slot.at(slot)->play_equip_animation();
        }
    }
}

void Player::jump() {

    if (is_grounded) {

        float delta_time = GetFrameTime();

        velocity.y += jump_speed * delta_time;
        is_grounded = false;
    }
}

void Player::add_velocity(Vector3 velocity_to_add) {

    if (abs(velocity.x) > max_speed) {
        velocity_to_add.x = 0.0f;
    }

    if (abs(velocity.z) > max_speed) {
        velocity_to_add.z = 0.0f;
    }

    if (!misc.noclip) {
        velocity_to_add.y = 0;
    }

    velocity += velocity_to_add;
}

void Player::decay_velocity(Vector3 decay_velocity) {

    if (velocity.x > 0) {
        velocity.x = std::max(0.0f, velocity.x * decay_velocity.x);
    } else if (velocity.x < 0) {
        velocity.x = std::min(0.0f, velocity.x * decay_velocity.x);
    }

    if (velocity.z > 0) {
        velocity.z = std::max(0.0f, velocity.z * decay_velocity.z);
    } else if (velocity.z < 0) {
        velocity.z = std::min(0.0f, velocity.z * decay_velocity.z);
    }
}

void Player::noclip_vertical_move() {

    if (misc.noclip) {
        if (IsKeyDown(KEY_SPACE)) {
            velocity += get_up() * move_speed * GetFrameTime();
        }

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            velocity -= get_up() * move_speed * GetFrameTime();
        }
    }
}

void Player::apply_colision(const std::vector<Collider> &map_colliders, const std::vector<Door> &map_doors) {

    for (auto &map_collider : map_colliders) {

        /* skip collision checking entirely if the object is too far away */
        float distance = Vector3Distance(collider.pos, map_collider.pos);
        if (distance > g_settings.coll_distance) {
            continue;
        }

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
                velocity.y = 0.0f;
            }
        } else {
            collider.is_colliding = false;
        }
    }

    /* check collision with moving door parts specifically */
    for (auto &door : map_doors) {

        float distance = Vector3Distance(collider.pos, door.collider_a.pos);

        if (distance > g_settings.coll_distance) {
            continue;
        }

        distance = Vector3Distance(collider.pos, door.collider_b.pos);

        if (distance > g_settings.coll_distance) {
            continue;
        }

        {

            MTV mtv;
            if (collider_check_collision(collider, door.collider_a, &mtv)) {

                collider.is_colliding = true;

                /* compute translation from collision */
                Vector3 mtv_direction = Vector3Normalize(mtv.axis);
                Vector3 translation = Vector3Scale(mtv_direction, mtv.depth);
                Vector3 to_player = Vector3Subtract(collider.pos, door.collider_a.pos);

                if (Vector3DotProduct(to_player, mtv_direction) < 0) {
                    translation = Vector3Negate(translation);
                }

                /* then apply it */
                collider.pos = Vector3Add(collider.pos, translation);

                /* check if its vertical from above (set grounded if true) */
                if (mtv.axis.y == 1 && translation.y > 0) {
                    is_grounded = true;
                    velocity.y = 0;
                } else {
                    collider.is_colliding = false;
                }
            }
        }

        {

            MTV mtv;
            if (collider_check_collision(collider, door.collider_b, &mtv)) {

                collider.is_colliding = true;

                /* compute translation from collision */
                Vector3 mtv_direction = Vector3Normalize(mtv.axis);
                Vector3 translation = Vector3Scale(mtv_direction, mtv.depth);
                Vector3 to_player = Vector3Subtract(collider.pos, door.collider_b.pos);

                if (Vector3DotProduct(to_player, mtv_direction) < 0) {
                    translation = Vector3Negate(translation);
                }

                /* then apply it */
                collider.pos = Vector3Add(collider.pos, translation);

                /* check if its vertical from above (set grounded if true) */
                if (mtv.axis.y == 1 && translation.y > 0) {
                    is_grounded = true;
                    velocity.y = 0;
                } else {
                    collider.is_colliding = false;
                }
            }
        }
    }
}

void Player::move(const std::vector<Collider> &map_colliders, const std::vector<Door> &map_doors) {

    float delta_time = GetFrameTime();
    Vector2 mouse_pos_delta = GetMouseDelta();

    camera_yaw(-mouse_pos_delta.x * camera_misc.mouse_sens * delta_time);
    camera_pitch(-mouse_pos_delta.y * camera_misc.mouse_sens * delta_time);

    if (IsKeyDown(KEY_SPACE)) {
        jump();
    }

    Vector3 forward = Vector3Normalize(get_forward());
    Vector3 right = Vector3Normalize(get_right());

    // allow player to go vertical looking up and down if noclipping
    if (!misc.noclip) {
        right.y = 0;
        forward.y = 0;
    }

    get_input();

    update_gravity();

    // add velocity forwards;
    Vector3 velocity_frwd = {};
    velocity_frwd += Vector3Normalize(forward) * move_speed * input.forwards * delta_time;
    add_velocity(velocity_frwd);

    // add velocity sideways;
    Vector3 velocity_sdw = {};
    velocity_sdw += Vector3Normalize(right) * side_speed * input.sideways * -1 * delta_time;
    add_velocity(velocity_sdw);

    // direction * move speed * (-1 ... 1) * delta_time
    //                                        ^ this leaves us with a percentage of total max speed
    // based on how long the player held the directional key
    // as in: slight tap = move slowly; held the key for a bit = fullspeed

    Vector3 decay_velocity_scale = {0.5, 1, 0.5};

    if (!is_grounded)
        decay_velocity_scale = {0.6, 1, 0.6};

    decay_velocity(decay_velocity_scale);

    noclip_vertical_move();

    collider.pos = Vector3Add(collider.pos, velocity);
    camera.target = Vector3Add(camera.target, Vector3Multiply(get_forward(), {10, 10, 10}));
    // ^ move camera target along with the player

    /* skip collision checking if noclipping */
    if (!misc.noclip) {
        apply_colision(map_colliders, map_doors);
    }
}

void Player::update_viewmodel() {

    if (inventory.selected_slot == 0) {
        return;
    }

    inventory.slot.at(inventory.selected_slot)->update(share_data);
}

void Player::draw_viewmodel() {

    if (inventory.selected_slot == 0) {
        return;
    }

    inventory.slot.at(inventory.selected_slot)->draw(share_data);
}

void Player::update_share_data() {

    share_data = {
        .player_pos = collider.pos,
        .forward = get_forward(),
        .right = get_right(),
        .camera_pos = camera.position,
        .input_forward = input.forwards,
        .input_sideways = input.sideways,
        .velocity = velocity,
    };
}

void Player::update(const std::vector<Collider> &map_colliders, const std::vector<Door> &map_doors) {

    if (misc.noclip) {
        misc.no_gravity = true;
    }
    if (!misc.noclip) {
        misc.no_gravity = false;
    }

    update_viewmodel();
    update_camera();

    move(map_colliders, map_doors);

    update_camera();
    update_share_data();
    update_viewmodel();

    if (IsKeyPressed(KEY_F3)) {
        misc.show_debug = !misc.show_debug;
        lognest_debug("[Player] Toggled debug menu '%s' -> '%s'.", bool_to_string(!misc.show_debug),
                      bool_to_string(misc.show_debug));
    }

#ifdef DEBUG

    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsKeyPressed(KEY_N)) {
            misc.noclip = !misc.noclip;
            lognest_debug("[Player] Toggle NoClip '%s' -> '%s'",
                          bool_to_string(!misc.noclip), bool_to_string(misc.noclip));
        }

        if (IsKeyPressed(KEY_B)) {

            lognest_debug("[Player] all items given.");

            give_item(1, ITEM_SHOTGUN);
            give_item(2, ITEM_AXE);
            give_item(3, ITEM_CABELA);
        }
    }

#endif // DEBUG
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

void Player::clear_inventory() {

    static EmptyItem empty_item;

    for (auto &itemptr : inventory.slot) {
        itemptr = &empty_item;
    }
}

void Player::give_item(size_t slot, PLAYER_ITEMS item) {

    switch (item) {
    case ITEM_EMPTY:
        return;
        break;

    case ITEM_COUNT:
        lognest_error("Unreachable case %s:%s.", __FILE__, __LINE__);
        assert(0);
        break;

    case ITEM_SHOTGUN:
        inventory.slot.at(slot) = new Shotgun();
        inventory.slot.at(slot)->play_equip_animation();
        break;

    case ITEM_AXE:
        inventory.slot.at(slot) = new Axe();
        inventory.slot.at(slot)->play_equip_animation();
        break;

    case ITEM_CABELA:
        inventory.slot.at(slot) = new Cabela();
        inventory.slot.at(slot)->play_equip_animation();
        break;

    default:
        return;
    }

    int pickup_index = GetRandomValue(0, 2);
    PlaySound(g_sounds.item_pickup_sound[pickup_index]);

    lognest_debug(" ┗>[Player] Gave '%s' at slot '%zu' to Player", get_item_as_cstr(item), slot);
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
                                      "is_colliding: %s\n"
                                      "selected_item: %d\n"
                                      "key_type: %s",
                                      collider.pos.x, collider.pos.y, collider.pos.z,
                                      input.forwards, input.sideways,
                                      width, 2, velocity.x,
                                      width, 2, velocity.y,
                                      width, 2, velocity.z,
                                      bool_to_string(is_grounded),
                                      bool_to_string(collider.is_colliding),
                                      inventory.selected_slot,
                                      get_key_as_cstr(current_key));

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

    rlPushMatrix();
    {
        rlTranslatef(collider.pos.x, collider.pos.y, collider.pos.z);

        BoundingBox player_bounding_box = GetModelBoundingBox(collider.model);
        DrawBoundingBox(player_bounding_box, GREEN);
    }
    rlPopMatrix();
}

void Player::draw() {
    draw_viewmodel();
    debug_3d();
}

void Player::draw_hud() {
    debug();

    /* crosshair */
    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 2, WHITE);

    // DrawRectangleLines(GetScreenWidth() / 2 + (GetScreenWidth() / 2) - 60, GetScreenHeight() / 2 + 150, 50, 50, WHITE);

    // DrawText(TextFormat("%d", inventory.selected_slot), GetScreenWidth() / 2, GetScreenHeight() / 2, 50, WHITE);

#ifdef DEBUG
    DrawText("Debug build", GetScreenWidth() - 150, GetScreenHeight() - 100, 20, WHITE);
#endif // DEBUG

    /*DrawText(TextFormat("%d", gameplay.health),*/
    /*(GetScreenWidth() / 32), GetScreenHeight() / 2 + 150, 50, GetColor(0xFF0000FF));*/

    /*DrawText(TextFormat("%d", gameplay.ammo),*/
    /*(GetScreenWidth() - 150), GetScreenHeight() / 2 + 150, 50, LIGHTGRAY);*/
}
