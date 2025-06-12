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

    ITEM_COUNT,

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

    Vector3 velocity;

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

    float rot;

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

    virtual void play_equip_animation() = 0;

    WF_timer equip_time;
};

class EmptyItem : public Item {
  public:
    Vector3 pos;

    EmptyItem();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    void play_equip_animation() override;

    WF_timer equip_time;
};

class Shotgun : public Item {
  public:
    Vector3 pos;

    Shotgun();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    WF_timer equip_time;

    void play_equip_animation() override;
};

class Axe : public Item {
  public:
    Vector3 pos;

    Axe();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    WF_timer equip_time;

    void play_equip_animation() override;
};

class Cabela : public Item {
  public:
    Vector3 pos;

    Cabela();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    WF_timer equip_time;

    void play_equip_animation() override;
};

#endif // !ITEMS_H_
