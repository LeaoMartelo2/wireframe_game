#include "collision.h"
#include "include/lognest.h"

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D(Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D(Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D(Vector3Zero(), {0, 0, distance}, BLUE);
}

void Geometry::draw(void) {

    DrawModel(model, pos, 1, WHITE);

    lognest_warn("single geometry drawn");
}

void Floor::draw(void) {

    DrawModel(model, pos, 1, WHITE);

    lognest_warn("single floor drawn");
}

void Trigger::draw(void) {

    DrawModel(model, pos, 1, ColorAlpha(ORANGE, 0.5));

    lognest_warn("single trigger drawn");
}
