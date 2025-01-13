#include "geometry.h"

void draw_reference_point() {
    float distance = 10000.0f;

    DrawLine3D((Vector3)Vector3Zero(), {distance, 0, 0}, RED);
    DrawLine3D((Vector3)Vector3Zero(), {0, distance, 0}, GREEN);
    DrawLine3D((Vector3)Vector3Zero(), {0, 0, distance}, BLUE);
}
