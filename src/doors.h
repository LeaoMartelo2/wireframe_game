#ifndef DOORS_H_
#define DOORS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "collision.h"

class Door {

  public:
    Collider collider_a;
    Collider collider_b;

    struct {
        Vector3 pos;
        Vector3 size;
    } open_trigger;

    struct {
        Vector3 pos_a;
        Vector3 pos_b;
    } open_pos;

    bool open;

    Door();
    ~Door();

    void update(const Collider &player);
    void draw();
};

#endif // !DOORS_H_
