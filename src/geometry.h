#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"

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

#endif // !GEOMETRY_H_
