#include "collision.h"
#include "wireframe.h"
#include <cmath>
#include <float.h>
#include <stddef.h>
#include <stdlib.h>

Collider::Collider() {};
Collider::~Collider() {};

void Collider::populate() {

    half_extents = {size.x / 2, size.y / 2, size.z / 2};
    mesh = GenMeshCube(size.x, size.y, size.z);
    model = LoadModelFromMesh(mesh);
    rotation = MatrixIdentity();
}

float Collider::get_projection_radius(Vector3 axis) const {

    Vector3 local_x = {rotation.m0, rotation.m4, rotation.m8};
    Vector3 local_y = {rotation.m1, rotation.m5, rotation.m9};
    Vector3 local_z = {rotation.m2, rotation.m6, rotation.m10};

    return fabs(Vector3DotProduct(local_x, axis)) * half_extents.x +
           fabs(Vector3DotProduct(local_y, axis)) * half_extents.y +
           fabs(Vector3DotProduct(local_z, axis)) * half_extents.z;
}

float Collider::get_max_radius() const {
    return Vector3Length(half_extents);
}

void Collider::draw() const {

    DrawModel(model, pos, 1.0, color);
    // DrawModelWires(model, pos, 1.0, outline_color);
    draw_wireframe(model, pos, 1.0, outline_color, 0.3);
}

bool collider_check_collision(const Collider &collider1, const Collider &collider2, MTV *mtv) {

    Vector3 axes[3] = {
        {1, 0, 0}, /* x axis */
        {0, 1, 0}, /* y axis */
        {0, 0, 1}, /* z axis*/
    };

    float min_overlap = INFINITY;
    Vector3 min_axis = {0};

    for (int k = 0; k < 3; k++) {
        Vector3 axis = axes[k];

        float proj1 = Vector3DotProduct(collider1.pos, axis);
        float radius1 = collider1.get_projection_radius(axis);
        float min1 = proj1 - radius1;
        float max1 = proj1 + radius1;

        float proj2 = Vector3DotProduct(collider2.pos, axis);
        float radius2 = collider2.get_projection_radius(axis);
        float min2 = proj2 - radius2;
        float max2 = proj2 + radius2;

        float overlap = fminf(max1, max2) - fmaxf(min1, min2);

        if (overlap <= 0) {
            return false;
        }

        if (overlap < min_overlap) {
            min_overlap = overlap;
            min_axis = axis;
        }
    }

    mtv->axis = min_axis;
    mtv->depth = min_overlap;
    return true;
}
