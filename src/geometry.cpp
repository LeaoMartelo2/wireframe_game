#include "collision.h"

#define GEOMETRY_COLOR DARKGRAY
#define FLOOR_COLOR GetColor(0x181818FF)

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D(Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D(Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D(Vector3Zero(), {0, 0, distance}, BLUE);
}

void Geometry::draw(void) {

    DrawModel(model, pos, 1, GEOMETRY_COLOR);
    DrawModelWires(model, pos, 1, RED);
}

void Floor::draw(void) {

    DrawModel(model, pos, 1, FLOOR_COLOR);
    DrawModelWires(model, pos, 1, RED);
}

void Trigger::draw(void) {

    DrawModel(model, pos, 1, ColorAlpha(ORANGE, 0.5));
}
