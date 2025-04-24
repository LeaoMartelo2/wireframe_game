#ifndef COLLISION_H_
#define COLLISION_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string>

class Collider {
  public:
    Vector3 *transformed_points;
    Vector3 *not_transformed;
    int num_points;
    Vector3 *normals;
    int num_normals;

    Vector3 size;
    Vector3 pos;
    Mesh mesh;
    Model model;

    virtual ~Collider();

    /* Populate Mesh and Model data */
    void populate(void);

    /* make these purely virtual functions */
    virtual void draw(void) = 0;
};

class Geometry : public Collider {

  public:
    Geometry();
    ~Geometry();

    void draw(void) override;
};

class Floor : public Collider {
  public:
    Floor();
    ~Floor();

    void draw(void) override;
};

typedef enum {
    TRIGGER_TELEPORT = 0,
    TRIGGER_LOADLEVEL,
    TRIGGER_GOTO_SCENE
} TRIGGER_TYPES;

typedef struct {
    Vector3 teleport;
    std::string levelname;
    size_t scene_id;
} trigger_info;

class Trigger : public Collider {

  public:
    int type;
    trigger_info info;

    Trigger();
    ~Trigger();
    void draw(void) override;
};

#endif // !COLLISION_H_
