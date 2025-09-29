#include "enemy_load.h"
#include "enemies.h"

Enemy *load_enemy_grunt(Vector3 pos) {

    const Vector3 grunt_hitbox_size = {10, 15, 10};
    const float grunt_turning_speed = 2.6f;

    Enemy *enemy = new Enemy;
    enemy->load();

    enemy->turning_speed = grunt_turning_speed;

    enemy->hitbox.pos = pos;
    enemy->hitbox.size = grunt_hitbox_size;

    /* enemy body parts */

    Enemy_body_parts *head = new Enemy_body_parts;
    head->update_pos = false;
    head->model = g_assets.cabela;
    head->custom_drawing = true;
    head->custom_drawing_function = draw_cabela;
    head->offset = {0, 5, 0};
    head->angle_offset = 180;
    head->custom_scale = {10, 10, 10};
    head->has_wireframe = false;
    head->face_player = true;
    enemy->body_models.push_back(head);

    Enemy_body_parts *body = new Enemy_body_parts;
    body->update_pos = false;
    body->model = LoadModelFromMesh(GenMeshCube(7.0f, 10.0f, 5.0f));
    body->offset = {0, 0, 0};
    body->has_wireframe = true;
    body->face_player = true;
    enemy->body_models.push_back(body);

    return enemy;
}
