#include "collision.h"

Collider::~Collider() {};

void Collider::populate(void) {

    mesh = GenMeshCube(size.x, size.y, size.z);
    model = LoadModelFromMesh(mesh);
}

Geometry::Geometry() {};
Geometry::~Geometry() {};

Floor::Floor() {};
Floor::~Floor() {};

Trigger::Trigger() {};
Trigger::~Trigger() {};
