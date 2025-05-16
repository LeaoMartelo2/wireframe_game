#ifndef ITEMS_H_
#define ITEMS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include <math.h>

#define FILL_COLOR GetColor(0x181818FF)

class Shotgun {
  public:
    bool enabled = false;
    Vector3 pos;

    Shotgun();

    void update(Vector3 forward, Vector3 right, Vector3 player_pos);
    void draw(Vector3 camera_pos, float input_sideways);
};

class Axe {
  public:
    bool enabled = false;
    Vector3 pos;

    Axe();

    void update(Vector3 forward, Vector3 right, Vector3 player_pos);
    void draw(Vector3 camera_pos, float input_forward, Vector3 player_right);
};

class Cabela {
  public:
    bool enabled = false;
    Vector3 pos;

    Cabela();

    void update(Vector3 forward, Vector3 right, Vector3 player_pos);
    void draw(Vector3 camera_pos);
};

#endif // !ITEMS_H_
