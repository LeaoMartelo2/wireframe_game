#ifndef DOORS_H_
#define DOORS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "collision.h"
#include "items.h"

typedef enum : size_t {
    DOORKEY_BLUE,
    DOORKEY_YELLOW,
    DOORKEY_GREEN,
    DOORKEY_NONE,

} DOORKEY_TYPE;

class Door {

  public:
    DOORKEY_TYPE key_type;

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
    bool finished_oppening = false;

    Door();
    ~Door();

    void set_key_type(std::string key_type);

    void update(const Collider &player, DOORKEY_TYPE *player_key);
    void draw() const;
};

Color door_get_color_from_keytype(std::string ktype);

Color get_keycolor_from_keytipe_enum(DOORKEY_TYPE key);

DOORKEY_TYPE get_keytpe_from_string(const std::string key_type);

const char *get_key_as_cstr(DOORKEY_TYPE key_type);

DOORKEY_TYPE get_keytype_from_color(const Color key_color);

class DroppedKey {
  public:
    Vector3 pos;
    Vector3 size;

    struct {

        Vector3 pos;
        Vector3 size;
    } collect_trigger;

    DOORKEY_TYPE type;
    Color key_color;

    float rot;

    DroppedKey();

    void load();
    void draw();
    int update(Vector3 player_pos, Vector3 player_size, DOORKEY_TYPE *player_key);
};

#endif // !DOORS_H_
