#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include <stdlib.h>

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
        size_t size;
        size_t capacity;
} Geometry_Array;

void geometry_array_init(Geometry_Array *arr);

void geometry_array_push(Geometry_Array *arr, const Geometry *geometry);

Geometry *geometry_array_get(Geometry_Array *arr, size_t index);

void geometry_array_free(Geometry_Array *arr);

void draw_world(Geometry_Array *map_geometry);

#endif // !GEOMETRY_H_
