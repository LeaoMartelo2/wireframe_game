#include "enemies.h"
#include "misc.h"
#include "wireframe.h"

const Vector3 ROTATION_AXIS = {0, 1, 0};
const Vector3 SCALE = {1, 1, 1};
const float NO_ROTATION = 0;
const float WIREFRAME_WIDTH = 0.3f;
const Color WIREFRAME_COLOR = BLACK;
const Color MODEL_COLOR = WHITE;

Enemy::Enemy() {};
Enemy::~Enemy() {};

Vector3 Enemy::get_forward() { return Vector3Normalize(Vector3Subtract(look_pos, hitbox.pos)); }

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

    forward = get_forward();
    forward.y = 0.0f;
    forward = Vector3Normalize(forward);

    angle = atan2f(forward.x, forward.z) * RAD2DEG;
}

void Enemy::draw(Camera *camera) {

    UNUSED(camera);

    for (size_t i = 0; i < body_models.size(); ++i) {

        if (body_models[i]->face_player) {
            DrawModelEx(body_models[i]->model, Vector3Add(hitbox.pos, body_models[i]->offset), ROTATION_AXIS, angle, SCALE, MODEL_COLOR);
            draw_wireframe_ex(body_models[i]->model, Vector3Add(hitbox.pos, body_models[i]->offset),
                              ROTATION_AXIS, angle, SCALE, WIREFRAME_COLOR, WIREFRAME_WIDTH);

        } else {

            DrawModelEx(body_models[i]->model, Vector3Add(hitbox.pos, body_models[i]->offset), ROTATION_AXIS, NO_ROTATION, SCALE, MODEL_COLOR);
            draw_wireframe_ex(body_models[i]->model, Vector3Add(hitbox.pos, body_models[i]->offset),
                              ROTATION_AXIS, NO_ROTATION, SCALE, WIREFRAME_COLOR, WIREFRAME_WIDTH);
        }

    }

#ifdef DEBUG

    DrawCubeWiresV(hitbox.pos, hitbox.size, ORANGE);

    // center of hitbox
    //DrawSphereWires(hitbox.pos, 1.5f, 5, 5, BLUE);
    DrawSphere(hitbox.pos, 1.0f, BLUE);

    // forward direction
    // DrawSphere(Vector3Scale(forward, 5.0f) + hitbox.pos, 1, BLUE);
    DrawLine3D(hitbox.pos, Vector3Add(hitbox.pos, Vector3Scale(forward, 15.0f)), YELLOW);

#endif // !DEBUG
}
