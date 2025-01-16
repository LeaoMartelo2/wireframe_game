#include "geometry.h"

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D((Vector3)Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D((Vector3)Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D((Vector3)Vector3Zero(), {0, 0, distance}, BLUE);
}

void geometry_draw(Geometry *geometry) {

    DrawModel(geometry->model, geometry->pos, 1.0f, DARKGRAY);
    DrawModelWires(geometry->model, geometry->pos, 1.0f, RED);
}

void floor_draw(Floor *floor) {
    DrawModel(floor->model, floor->pos, 1.0f, GetColor(0x181818FF));
    /*DrawModelWires(floor->model, floor->pos, 1.0f, RED);*/
}
