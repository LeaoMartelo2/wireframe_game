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

void draw_reference_point(void);

void geometry_draw(Geometry *geometry);

#endif // !GEOMETRY_H_
