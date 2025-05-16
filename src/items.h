#ifndef ITEMS_H_
#define ITEMS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include <math.h>

typedef enum PLAYER_ITEMS {
    ITEM_SHOTGUN = 0,
    ITEM_AXE,
    ITEM_CABELA,

    ITEM_COUNT,

} PLAYER_ITEMS;

#define FILL_COLOR GetColor(0x181818FF)

class DroppedItem {
  public:
    Vector3 pos;
    Vector3 size;

    struct {

        Vector3 pos;
        Vector3 size;
    } collect_trigger;

    bool collected = false;

    PLAYER_ITEMS type;

    DroppedItem();

    void draw();
    int update(Vector3 player_pos, Vector3 player_size);
};

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
