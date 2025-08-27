#include "enemies.h"
#include "wireframe.h"
#include "misc.h"

Enemy::Enemy() {};
Enemy::~Enemy() {};

void Enemy::load() {

    hitbox.pos = {-50, 7, -30};
    hitbox.size = {10, 15, 10};

    Enemy_body_parts test;

    test.model = LoadModelFromMesh(GenMeshCube(7, 15, 7));

    body_models.push_back(test);

    Enemy_body_parts test2;

    test2.model = LoadModelFromMesh(GenMeshCube(1, 5, 7));

    body_models.push_back(test2);

    Enemy_body_parts test3;

    test3.model = LoadModelFromMesh(GenMeshCube(7, 1, 1));

    body_models.push_back(test3);
}

void Enemy::update() {};

void Enemy::draw(Camera *camera) {

        
    UNUSED(camera);


    for (size_t i = 0; i < body_models.size(); ++i) {

        //       DrawModel(body_models[i].model, hitbox.pos, 1, WHITE);
        draw_wireframe(body_models[i].model, hitbox.pos, 1, BLACK, 0.1f);
    }

#ifdef DEBUG

    DrawCubeWiresV(hitbox.pos, hitbox.size, ORANGE);

    DrawSphereWires(hitbox.pos, 1.5f, 5, 5, BLUE);

#endif // !DEBUG
}
