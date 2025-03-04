#include "geometry.h"

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D(Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D(Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D(Vector3Zero(), {0, 0, distance}, BLUE);
}

void geometry_outline(Geometry *geometry, float size, Color color) {

    Vector3 right_front = geometry->pos;
    right_front.x -= geometry->size.x / 2.0;
    right_front.z += geometry->size.z / 2.0;

    DrawCube(right_front, size, geometry->size.y, size, color);

    // left front
    DrawCube({geometry->pos.x + geometry->size.x / 2, right_front.y, right_front.z},
             size, geometry->size.y, size, color);

    // right back
    DrawCube({right_front.x, right_front.y, geometry->pos.z - geometry->size.z / 2},
             size, geometry->size.y, size, color);

    // left back
    DrawCube({geometry->pos.x + geometry->size.x / 2, right_front.y, geometry->pos.z - geometry->size.z / 2},
             size, geometry->size.y, size, color);

    Vector3 top_right = geometry->pos;
    top_right.y += geometry->size.y / 2;
    top_right.x -= geometry->size.x / 2;

    DrawCube(top_right, size, size, geometry->size.z, color);

    // bottom right
    DrawCube({top_right.x, geometry->pos.y - geometry->size.y / 2, top_right.z},
             size, size, geometry->size.z, color);

    // top left
    DrawCube({geometry->pos.x + geometry->size.x / 2, top_right.y, top_right.z},
             size, size, geometry->size.z, color);

    // bottom left
    DrawCube({geometry->pos.x + geometry->size.x / 2, geometry->pos.y - geometry->size.y / 2, top_right.z},
             size, size, geometry->size.z, color);

    Vector3 top_front = geometry->pos;
    top_front.z += geometry->size.z / 2;
    top_front.y += geometry->size.y / 2;

    DrawCube(top_front, geometry->size.x, size, size, color);

    // bottom front
    DrawCube({top_front.x, geometry->pos.y - geometry->size.y / 2, top_front.z},
             geometry->size.x, size, size, color);

    // top back
    DrawCube({top_front.x, top_front.y, geometry->pos.z - geometry->size.z / 2},
             geometry->size.x, size, size, color);

    // bottom back

    DrawCube({top_front.x, geometry->pos.y - geometry->size.y / 2, geometry->pos.z - geometry->size.z / 2},
             geometry->size.x, size, size, color);
}

void geometry_draw(Geometry *geometry) {

    DrawCubeV(geometry->pos, geometry->size, DARKGRAY);
    geometry_outline(geometry, 0.3f, RED);
}

void floor_outline(Floor *floor, float size, Color color) {

    Vector3 right_front = floor->pos;
    right_front.x -= floor->size.x / 2.0;
    right_front.z += floor->size.z / 2.0;

    DrawCube(right_front, size, floor->size.y, size, color);

    // left front
    DrawCube({floor->pos.x + floor->size.x / 2, right_front.y, right_front.z},
             size, floor->size.y, size, color);

    // right back
    DrawCube({right_front.x, right_front.y, floor->pos.z - floor->size.z / 2},
             size, floor->size.y, size, color);

    // left back
    DrawCube({floor->pos.x + floor->size.x / 2, right_front.y, floor->pos.z - floor->size.z / 2},
             size, floor->size.y, size, color);

    Vector3 top_right = floor->pos;
    top_right.y += floor->size.y / 2;
    top_right.x -= floor->size.x / 2;

    DrawCube(top_right, size, size, floor->size.z, color);

    // bottom right
    DrawCube({top_right.x, floor->pos.y - floor->size.y / 2, top_right.z},
             size, size, floor->size.z, color);

    // top left
    DrawCube({floor->pos.x + floor->size.x / 2, top_right.y, top_right.z},
             size, size, floor->size.z, color);

    // bottom left
    DrawCube({floor->pos.x + floor->size.x / 2, floor->pos.y - floor->size.y / 2, top_right.z},
             size, size, floor->size.z, color);

    Vector3 top_front = floor->pos;
    top_front.z += floor->size.z / 2;
    top_front.y += floor->size.y / 2;

    DrawCube(top_front, floor->size.x, size, size, color);

    // bottom front
    DrawCube({top_front.x, floor->pos.y - floor->size.y / 2, top_front.z},
             floor->size.x, size, size, color);

    // top back
    DrawCube({top_front.x, top_front.y, floor->pos.z - floor->size.z / 2},
             floor->size.x, size, size, color);

    // bottom back

    DrawCube({top_front.x, floor->pos.y - floor->size.y / 2, floor->pos.z - floor->size.z / 2},
             floor->size.x, size, size, color);
}

void floor_draw(Floor *floor) {
    DrawCubeV(floor->pos, floor->size, GetColor(0x181818FF));
    floor_outline(floor, 0.3f, RED);
}
