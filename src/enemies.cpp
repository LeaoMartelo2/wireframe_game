#include "enemies.h"
#include "misc.h"
#include "wireframe.h"

Enemy::Enemy() {};
Enemy::~Enemy() {};

void Enemy::load() {

    hitbox.pos = {-50, 7, -30};
    hitbox.size = {10, 15, 10};

    Enemy_body_parts *test = new Enemy_body_parts;
    test->model = LoadModelFromMesh(GenMeshCube(7, 7.5, 7));
    test->face_player = false;
    test->offset = {0, -3.5, 0};
    body_models.push_back(test);

    Enemy_body_parts *test2 = new Enemy_body_parts;
    test2->model = LoadModelFromMesh(GenMeshCube(1, 5, 7));
    test2->offset = {forward.x, 0, 5};
    test2->face_player = true;
    body_models.push_back(test2);

    Enemy_body_parts *test3 = new Enemy_body_parts;
    test3->model = LoadModelFromMesh(GenMeshCube(7, 1, 1));
    body_models.push_back(test3);

}

void Enemy::update(GenericPlayerData_share player) {

    look_pos = Vector3Add(player.player_pos, {0, 4.5f, 0});

    forward = Vector3Normalize(Vector3Subtract(look_pos, hitbox.pos));

    for (size_t i = 0; i < body_models.size(); ++i) {

        if(body_models[i]->face_player){
            body_models[i]->offset = Vector3Scale(forward, 5.0f);
        }

    }
}

void Enemy::draw(Camera *camera) {

    UNUSED(camera);

    DrawSphereWires(look_pos, 0.5f, 10, 10, YELLOW);

    DrawLine3D(look_pos, hitbox.pos, YELLOW);

    DrawSphere(Vector3Scale(forward, 5.0f) + hitbox.pos, 1, BLUE);

    for (size_t i = 0; i < body_models.size(); ++i) {

        DrawModel(body_models[i]->model, Vector3Add(hitbox.pos, body_models[i]->offset), 1, WHITE);
        draw_wireframe(body_models[i]->model, Vector3Add(hitbox.pos, body_models[i]->offset), 1, BLACK, 0.3f);
    }

#ifdef DEBUG

    DrawCubeWiresV(hitbox.pos, hitbox.size, ORANGE);

    DrawSphereWires(hitbox.pos, 1.5f, 5, 5, BLUE);

#endif // !DEBUG
}
