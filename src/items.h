#ifndef ITEMS_H_
#define ITEMS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"

class Item {

  public:
    Vector3 pos;
    Vector3 size;

    Model *model;

    Item();
    ~Item();
};

class Shotgun : public Item {};

#endif // !ITEMS_H_
