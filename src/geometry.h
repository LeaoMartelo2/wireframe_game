#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 10

typedef struct Geometry {
    Vector3 size;
    Vector3 pos;
    Mesh mesh;
    Model model;
} Geometry;

typedef struct Prop {
    Vector3 size;
    Vector3 pos;
    Mesh mesh;
    Model model;
    bool bounce;
    bool rotate;
} Prop;

typedef struct {
    Geometry *data;
    int size;
    int capacity;
} Geometry_Array;

Geometry_Array *geometry_array_setup();
void geometry_array_push(Geometry_Array *arr, const Geometry geometry);
Geometry geometry_array_get(Geometry_Array *arr, int index);
void geometry_array_free(Geometry_Array *arr);

void draw_reference_point();

void draw_world(Geometry_Array *map_geometry);

void test_cube(Geometry_Array *map_geometry);

#endif // !GEOMETRY_H_
