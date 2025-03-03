#include "geometry.h"

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D(Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D(Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D(Vector3Zero(), {0, 0, distance}, BLUE);
}

void geometry_draw(Geometry *geometry) {

    /*DrawModel(geometry->model, geometry->pos, 1.0f, DARKGRAY);*/
    DrawCubeV(geometry->pos, geometry->size, DARKGRAY);
    /*DrawModelWires(geometry->model, geometry->pos, 1.0f, RED);*/
    DrawCubeWiresV(geometry->pos, geometry->size, RED);
}

void floor_draw(Floor *floor) {
    /*DrawModel(floor->model, floor->pos, 1.0f, GetColor(0x181818FF));*/
    DrawCubeV(floor->pos, floor->size, GetColor(0x181818FF));
    DrawCubeWiresV(floor->pos, floor->size, RED);
}
