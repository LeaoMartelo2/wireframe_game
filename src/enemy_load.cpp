#include "enemy_load.h"

void load_enemy_grunt(Enemy *enemy) {
    enemy->hitbox.pos = {-50, 7, -30};
    enemy->hitbox.size = {10, 15, 10};

    Enemy_body_parts *test = new Enemy_body_parts;
    test->model = LoadModelFromMesh(GenMeshCube(7, 7.5, 7));
    test->face_player = true;
    test->offset = {0, -3.5, 0};
    test->update_pos = false;
    enemy->body_models.push_back(test);

    Enemy_body_parts *test2 = new Enemy_body_parts;
    test2->model = g_assets.shotgun;
    test2->has_wireframe = false;
    test2->custom_drawing = true;
    test2->custom_drawing_function = draw_shotgun;
    test2->custom_scale = {10, 10, 10};
    test2->offset = Vector3Scale(enemy->get_right(), -5.0f);
    test2->update_pos = true;
    test2->distance = 5.0f;
    test2->update_pos_function = update_pos_vec;
    test2->face_player = true;
    enemy->body_models.push_back(test2);

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
    enemy->body_models.push_back(test3);

    enemy->turning_speed = 2.6f;
    enemy->loaded = true;
}

void load_enemy_test(Enemy *enemy) {

    enemy->hitbox.pos = {-50, 7, -30};
    enemy->hitbox.size = {10, 15, 10};

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
    enemy->body_models.push_back(test3);

    /*
    Enemy_body_parts *test = new Enemy_body_parts;
    test->model = LoadModelFromMesh(GenMeshCube(7, 7.5, 7));
    test->face_player = true;
    test->offset = {0, -3.5, 0};
    test->angle_offset = 0.0f;
    test->update_pos = false;
    test->custom_scale = {1, 1, 1};
    enemy->body_models.push_back(test);

    */

    enemy->turning_speed = 2.6f;
    enemy->loaded = true;
}

Enemy *load_enemy_test_ptr() {

    Enemy *enemy = new Enemy;

    enemy->hitbox.pos = {-50, 7, -30};
    enemy->hitbox.size = {10, 15, 10};

    enemy->angle = 0.0f;

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

    enemy->body_models.push_back(test3);

    enemy->turning_speed = 2.6f;
    enemy->loaded = true;

    return enemy;
}
