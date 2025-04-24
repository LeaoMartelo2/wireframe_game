#include "collision.h"
#include <cfloat>
#include <cstdlib>

Collider::~Collider() {};

void Collider::populate(void) {

    mesh = GenMeshCube(size.x, size.y, size.z);
    model = LoadModelFromMesh(mesh);
}

void Collider::add_normal(Vector3 normal) {
    normals.push_back(normal);
}

void get_normals(Mesh mesh, std::vector<Vector3> *normals) {

    for (int i = 0; i < mesh.vertexCount; i += 3) {
        normals->push_back({mesh.normals[i],
                            mesh.normals[i + 1],
                            mesh.normals[i + 2]});
    }
}

void setup_collider_mesh(Collider *c, Mesh mesh) {

    c->num_points = mesh.vertexCount;

    int vertex = 0;

    for (int i = 0; i < c->num_points; i++) {
        c->not_transformed.push_back({mesh.vertices[vertex],
                                      mesh.vertices[vertex + 1],
                                      mesh.vertices[vertex + 2]});
        vertex += 3;
    }

    c->num_normals = mesh.triangleCount;
    get_normals(mesh, &c->normals);
}

void get_min_max(Collider *b, Vector3 axis, float *min, float *max) {

    *min = Vector3DotProduct(b->transformed_points[0], axis);
    *max = *min; // initialize max with same value as min

    for (int i = 1; i < b->num_points; i++) {

        float dot = Vector3DotProduct(b->transformed_points[i], axis);

        // if dot smaller than min, thats the new min
        if (dot < *min) {
            *min = dot;
        }

        // if dot is bigger than max, thats the new max

        if (dot > *max) {
            *max = dot;
        }
    }
}

Vector3 get_middle_point(std::vector<Vector3> *verticies, int num_verticies) {

    float x, y, z;

    for (int i = 0; i < num_verticies; i++) {

        x += verticies[i].data()->x;
        y += verticies[i].data()->y;
        z += verticies[i].data()->z;
    }

    return {x / num_verticies,
            y / num_verticies,
            z / num_verticies};
}

bool check_collision(Collider *a, Collider *b, Vector3 *normal) {

    *normal = {0, 0, 0};
    float depth = FLT_MAX; // init depth as max it can be

    for (int i = 0; i < a->num_normals; i++) {
        float min1, max1;
        float min2, max2;

        get_min_max(a, a->normals[i], &min2, &max2);
        get_min_max(b, a->normals[i], &min1, &max1);

        if (max1 < min2 || max2 < min1) {
            return false; // no collision on this axis
        } else {
            float axis_depth = fminf(max2 - min1, max1 - min2);

            if (axis_depth < depth) {
                depth = axis_depth;
                *normal = a->normals[i];
            }
        }
    }

    for (int i = 0; i < b->num_normals; i++) {
        float min1, max1;
        float min2, max2;

        get_min_max(a, b->normals[i], &min2, &max2);
        get_min_max(b, b->normals[i], &min1, &max1);

        if (max1 < min2 || max2 < min1) {
            return false; // no collision on this axis
        } else {
            float axis_depth = fminf(max2 - min1, max1 - min2);

            if (axis_depth < depth) {
                depth = axis_depth;
                *normal = b->normals[i];
            }
        }
    }

    Vector3 direction = Vector3Subtract(get_middle_point(&a->transformed_points, a->num_points),
                                        get_middle_point(&b->transformed_points, b->num_points));

    if (Vector3DotProduct(direction, *normal) < 0.0f) {
        *normal = Vector3Negate(*normal);
    }
    *normal = Vector3Scale(*normal, depth);
    return true;
}

void update_collider(Vector3 parent, Collider *c) {

    for (int i = 0; i < c->num_points; i++) {
        c->transformed_points[i] = Vector3Add(c->not_transformed[i], parent);
    }
}

Geometry::Geometry() {};
Geometry::~Geometry() {};

Floor::Floor() {};
Floor::~Floor() {};

Trigger::Trigger() {};
Trigger::~Trigger() {};
