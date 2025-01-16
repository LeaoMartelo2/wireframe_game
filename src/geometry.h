#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "include/json.hpp"
#include <stdlib.h>

typedef struct Geometry {
    Vector3 size;
    Vector3 pos;
    Mesh mesh;
    Model model;
} Geometry;

void geometry_draw(Geometry *geometry);

typedef struct Prop {
    Vector3 size;
    Vector3 pos;
    Mesh mesh;
    Model model;
    bool bounce;
    bool rotate;
} Prop;

void draw_reference_point(void);

typedef struct Floor {
    Vector3 size;
    Vector3 pos;
    Mesh mesh;
    Model model;
} Floor;

void floor_draw(Floor *floor);

#endif // !GEOMETRY_H_
