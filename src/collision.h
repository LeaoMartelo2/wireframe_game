#ifndef COLLISION_H_
#define COLLISION_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>

class Collider {
  public:
    // array of points defining the transfortmed collider
    /*Vector3 *transformed_points;*/
    std::vector<Vector3> transformed_points;
    // array of points defining the collider
    /*Vector3 *not_transformed;*/
    std::vector<Vector3> not_transformed;
    int num_points;
    // normals array
    /*Vector3 *normals;*/
    std::vector<Vector3> normals;
    void add_normal(Vector3 normal);

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

void get_normals(Mesh mesh, std::vector<Vector3> *normals);
void setup_collider_mesh(Collider *c, Mesh mesh);
void get_min_max(Collider *b, Vector3 axis, float *min, float *max);
Vector3 get_middle_point(std::vector<Vector3> *verticies, int num_verticies);
bool check_collision(Collider *a, Collider *b, Vector3 *normal);
void update_collider(Vector3 parent, Collider *c);

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
