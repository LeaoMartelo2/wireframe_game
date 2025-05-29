#ifndef ITEMS_H_
#define ITEMS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include <math.h>
#include <stddef.h>

typedef enum PLAYER_ITEMS {
    ITEM_EMPTY = -1,
    ITEM_SHOTGUN = 0,
    ITEM_AXE,
    ITEM_CABELA,

    ITEM_COUNT [[maybe_unused]],

} PLAYER_ITEMS;

const char *get_item_as_cstr(PLAYER_ITEMS item);

#define FILL_COLOR GetColor(0x181818FF)

typedef struct {
    Vector3 player_pos;
    Vector3 forward;
    Vector3 right;

    Vector3 camera_pos;

    float input_forward;
    float input_sideways;

} GenericPlayerData_share;

class DroppedItem {
  public:
    Vector3 pos;
    Vector3 size;

    struct {

        Vector3 pos;
        Vector3 size;
    } collect_trigger;

    size_t player_slot;
    PLAYER_ITEMS type;

    long double rot;

    DroppedItem();

    void load();
    void draw();
    int update(Vector3 player_pos, Vector3 player_size);
};

class Item {

  public:
    Vector3 pos;

    virtual void update(GenericPlayerData_share data) = 0;
    virtual void draw(GenericPlayerData_share data) = 0;
};

class EmptyItem : public Item {
  public:
    Vector3 pos;

    EmptyItem();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;
};

class Shotgun : public Item {
  public:
    Vector3 pos;

    Shotgun();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;
};

class Axe : public Item {
  public:
    Vector3 pos;

    Axe();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;
};

class Cabela : public Item {
  public:
    Vector3 pos;

    Cabela();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;
};

#endif // !ITEMS_H_
