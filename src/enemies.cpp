#include "enemies.h"
#include "enemy_dispatch.h"
#include "wireframe.h"
#include <algorithm>

Enemy::Enemy() {};
Enemy::~Enemy() {

    for (auto &bp : body_models) {
        delete bp;
    }
};

Vector3 Enemy::get_forward() { return Vector3Normalize(Vector3Subtract(look_pos, hitbox.pos)); }
Vector3 Enemy::get_up() { return Vector3Normalize({0, hitbox.pos.y + 1.0f, 0}); }
Vector3 Enemy::get_right() { return Vector3Normalize(Vector3CrossProduct(get_forward(), get_up())); }

void Enemy::load() {

    hitbox.pos = {-50, 7, -30};
    hitbox.size = {10, 15, 10};

    Enemy_body_parts *test = new Enemy_body_parts;
    test->model = LoadModelFromMesh(GenMeshCube(7, 7.5, 7));
    test->face_player = true;
    test->offset = {0, -3.5, 0};
    test->update_pos = false;
    body_models.push_back(test);

    Enemy_body_parts *test2 = new Enemy_body_parts;
    test2->model = g_assets.shotgun;
    test2->has_wireframe = false;
    test2->custom_drawing = true;
    test2->custom_drawing_function = draw_shotgun;
    test2->custom_scale = {10, 10, 10};
    test2->offset = Vector3Scale(get_right(), -5.0f);
    test2->update_pos = true;
    test2->distance = 5.0f;
    test2->update_pos_function = update_pos_vec;
    test2->face_player = true;
    body_models.push_back(test2);

    Enemy_body_parts *test3 = new Enemy_body_parts;
    test3->update_pos = false;
    test3->model = g_assets.cabela;
    test3->custom_drawing = true;
    test3->custom_drawing_function = draw_cabela;
    test3->offset = {0, 5, 0};
    test3->angle_offset = 180;
    test3->custom_scale = {10, 10, 10};
    test3->has_wireframe = false;
    test3->face_player = true;
    body_models.push_back(test3);


    turning_speed = 2.6f;
    loaded = true;
}

void Enemy::update(GenericPlayerData_share player) {

    look_pos = Vector3Add(player.player_pos, {0, 4.5f, 0});

    forward = get_forward();
    forward.y = 0.0f;
    forward = Vector3Normalize(forward);

    float pred_angle = (atan2f(forward.x, forward.z)) * RAD2DEG;

    float angle_difference = pred_angle - angle;

    if (angle_difference > 180.0f) angle_difference -= 360.0f;
    if (angle_difference < -180.0f) angle_difference += 360.0f;

    //float turning_speed = 5.6f;

    angle += std::clamp(angle_difference, -turning_speed, turning_speed);

    for (size_t i = 0; i < body_models.size(); ++i) {
        if (body_models[i]->update_pos && body_models[i]->update_pos_function) {

            if (loaded) {
                body_models[i]->update_pos_function(body_models[i], get_right());
            }
        }
    }

}

void Enemy::draw() {


    for (size_t i = 0; i < body_models.size(); ++i) {

        Enemy_body_parts *part = body_models[i];

        float draw_angle = NO_ROTATION;

        if (part->face_player) draw_angle = angle + part->angle_offset;

        if (part->custom_drawing && part->custom_drawing_function) {
            part->custom_drawing_function(part, hitbox.pos, draw_angle);

        } else {

            DrawModelEx(part->model,
                        Vector3Add(hitbox.pos, part->offset),
                        ROTATION_AXIS,
                        draw_angle,
                        Vector3Multiply(SCALE, part->custom_scale),
                        MODEL_COLOR);

            if (part->has_wireframe) {

                draw_wireframe_ex(part->model,
                                  Vector3Add(hitbox.pos, part->offset),
                                  ROTATION_AXIS,
                                  draw_angle,
                                  Vector3Multiply(SCALE, part->custom_scale),
                                  WIREFRAME_COLOR,
                                  WIREFRAME_WIDTH);
            }
        }
    }

#ifdef DEBUG

    DrawCubeWiresV(hitbox.pos, hitbox.size, ORANGE);

    // center of hitbox
    // DrawSphereWires(hitbox.pos, 1.5f, 5, 5, BLUE);
    DrawSphere(hitbox.pos, 1.0f, ORANGE);


    /* orientation lines */
    Vector3 drw_forward = get_forward();
    drw_forward.y = 0;
    drw_forward = Vector3Normalize(drw_forward);
    draw_line3d_thick(hitbox.pos, Vector3Add(hitbox.pos, Vector3Scale(drw_forward, 15.0f)), 0.2f, RED);

    draw_line3d_thick(hitbox.pos, Vector3Add(hitbox.pos, Vector3Scale(get_up(), 15.0f)), 0.2f, GREEN);

    draw_line3d_thick(hitbox.pos, Vector3Add(hitbox.pos, Vector3Scale(get_right(), 15.0f)), 0.2f, BLUE);

#endif // !DEBUG
}
