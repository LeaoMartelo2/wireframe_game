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

enum class ITEM_SLOTS : size_t {
    SHOTGUN = 1,
    AXE = 2,
    CABELA = 3,  
};

enum class AMMO_TYPE : size_t {
    SHELLS,
    AXES,
    ENERGY,
};

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

    bool can_fire = true;
    WF_timer fire_time;
    virtual bool fire() = 0;

    struct {
        size_t damage;

        size_t ammo_capacity;
        size_t ammo_on_pickup;
        size_t ammo_to_fire;

        AMMO_TYPE ammo_type;

        float use_cooldown;

    } stats;
};

class EmptyItem : public Item {
  public:
    EmptyItem();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;
    bool fire() override;
    void play_equip_animation() override;
};

class Shotgun : public Item {
  public:
    Shotgun();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    WF_timer equip_time;

    void play_equip_animation() override;

    bool fire() override;
};

class Axe : public Item {
  public:
    Axe();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    bool fire() override;

    WF_timer equip_time;

    void play_equip_animation() override;
};

class Cabela : public Item {
  public:
    Cabela();

    void update(GenericPlayerData_share data) override;
    void draw(GenericPlayerData_share data) override;

    bool fire() override;

    WF_timer equip_time;

    void play_equip_animation() override;
};

#endif // !ITEMS_H_
