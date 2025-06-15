#ifndef DOORS_H_
#define DOORS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "collision.h"

typedef enum : size_t {
    DOORKEY_BLUE,
    DOORKEY_YELLOW,
    DOORKEY_RED,
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

const char *get_key_as_cstr(DOORKEY_TYPE key_type);

#endif // !DOORS_H_
