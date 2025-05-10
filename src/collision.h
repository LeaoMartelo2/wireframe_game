#ifndef COLLISION_H_
#define COLLISION_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "wireframe.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>

// minimal translaction vector
typedef struct MTV {
    Vector3 axis;
    float depth;
} MTV;

// Collider = Oriented Bounding Box
class Collider {
  public:
    Vector3 pos;          /* center of the OBB */
    Vector3 size;         /* width, height, lenght */
    Vector3 half_extents; /* half dimensions, so dosent have to recalculate it every time  */
    Matrix rotation;
    bool is_colliding = false;
    Mesh mesh;
    Model model;
    Color color;
    Color outline_color;

    Collider();
    ~Collider();

    /* initialize mesh, model data, and miscelanious */
    void populate();

    float get_projection_radius(Vector3 axis) const; /* calculate projection radious onto an axis */
    float get_max_radius() const;                    /* max radious for broad collision check */
    void draw() const;
};

/* SAT collision optimized for axis-aligned checks */
bool collider_check_collision(const Collider &collider1, const Collider &collider2, MTV *mtv);

#endif // !COLLISION_H_
