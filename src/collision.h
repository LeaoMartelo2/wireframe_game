#ifndef COLLISION_H_
#define COLLISION_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>

class Collider {
  public:
    Vector3 *transformed_points;
    Vector3 *not_transformed;
    int num_points;
    Vector3 *normals;
    int num_normals;
};

#endif // !COLLISION_H_
