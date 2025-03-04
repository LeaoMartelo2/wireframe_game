#include "geometry.h"

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D(Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D(Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D(Vector3Zero(), {0, 0, distance}, BLUE);
}

void geometry_outline(Geometry *geometry, float outline, Color color) {

    Vector3 right_front = geometry->pos;
    right_front.x -= geometry->size.x / 2.0;
    right_front.z += geometry->size.z / 2.0;

    DrawCube(right_front, outline, geometry->size.y, outline, color);

    // left front
    DrawCube({geometry->pos.x + geometry->size.x / 2, right_front.y, right_front.z},
             outline, geometry->size.y, outline, color);

    // right back
    DrawCube({right_front.x, right_front.y, geometry->pos.z - geometry->size.z / 2},
             outline, geometry->size.y, outline, color);

    // left back
    DrawCube({geometry->pos.x + geometry->size.x / 2, right_front.y, geometry->pos.z - geometry->size.z / 2},
             outline, geometry->size.y, outline, color);

    Vector3 top_right = geometry->pos;
    top_right.y += geometry->size.y / 2;
    top_right.x -= geometry->size.x / 2;

    DrawCube(top_right, outline, outline, geometry->size.z, color);

    // bottom right
    DrawCube({top_right.x, geometry->pos.y - geometry->size.y / 2, top_right.z},
             outline, outline, geometry->size.z, color);

    // top left
    DrawCube({geometry->pos.x + geometry->size.x / 2, top_right.y, top_right.z},
             outline, outline, geometry->size.z, color);

    // bottom left
    DrawCube({geometry->pos.x + geometry->size.x / 2, geometry->pos.y - geometry->size.y / 2, top_right.z},
             outline, outline, geometry->size.z, color);

    Vector3 top_front = geometry->pos;
    top_front.z += geometry->size.z / 2;
    top_front.y += geometry->size.y / 2;

    DrawCube(top_front, geometry->size.x, outline, outline, color);

    // bottom front
    DrawCube({top_front.x, geometry->pos.y - geometry->size.y / 2, top_front.z},
             geometry->size.x, outline, outline, color);

    // top back
    DrawCube({top_front.x, top_front.y, geometry->pos.z - geometry->size.z / 2},
             geometry->size.x, outline, outline, color);

    // bottom back

    DrawCube({top_front.x, geometry->pos.y - geometry->size.y / 2, geometry->pos.z - geometry->size.z / 2},
             geometry->size.x, outline, outline, color);
}

void geometry_draw(Geometry *geometry) {

    DrawCubeV(geometry->pos, geometry->size, DARKGRAY);
    /*DrawCubeWiresV(geometry->pos, geometry->size, RED);*/
    geometry_outline(geometry, 0.3f, RED);
}

void floor_draw(Floor *floor) {
    /*DrawModel(floor->model, floor->pos, 1.0f, GetColor(0x181818FF));*/
    DrawCubeV(floor->pos, floor->size, GetColor(0x181818FF));
    DrawCubeWiresV(floor->pos, floor->size, RED);
}
