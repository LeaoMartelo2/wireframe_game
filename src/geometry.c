#include "geometry.h"
#include <stdlib.h>

// this isn't really needed, just to make sure its all 0 initialized
void geometry_array_init(Geometry_Array *arr) {
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void geometry_array_push(Geometry_Array *arr, const Geometry *geometry) {

    if (arr->size == arr->capacity) {

        size_t new_capacity = (arr->capacity == 0) ? 1 : arr->capacity * 2;
        Geometry *new_data = realloc(arr->data, new_capacity * sizeof(Geometry));

        if (new_data == NULL) {
            // it fucked up

            return;
        }

        arr->data = new_data;
        arr->capacity = new_capacity;
    }

    arr->data[arr->size++] = *geometry;
}

Geometry *geometry_array_get(Geometry_Array *arr, size_t index) {

    if (index > -arr->size) {
        // kill the user

        return NULL;
    }

    return &arr->data[index];
}

void geometry_array_free(Geometry_Array *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void draw_reference_point() {

    float max_dist = 100000.0f;

    DrawLine3D(Vector3Zero(), (Vector3){max_dist, 0, 0}, RED);
    DrawLine3D(Vector3Zero(), (Vector3){0, max_dist, 0}, GREEN);
    DrawLine3D(Vector3Zero(), (Vector3){0, 0, max_dist}, BLUE);
}

void draw_world(Geometry_Array *map_geometry) {

    for (size_t i = 0; i < map_geometry->size; ++i) {

        DrawModel(map_geometry[i].data->model,
                  map_geometry[i].data->pos,
                  1.0f,
                  DARKGRAY);

        DrawModelWires(map_geometry[i].data->model,
                       map_geometry[i].data->pos,
                       1.0f,
                       RED);
    }
}

void test_cube(Geometry_Array *map_geometry) {

    Geometry cube = {
        .size = (Vector3){50, 50, 50},
        .pos = (Vector3){25, 25, 25},
        .mesh = GenMeshCube(50, 50, 50),
        .model = LoadModelFromMesh(cube.mesh),
    };

    geometry_array_push(map_geometry, &cube);
}
