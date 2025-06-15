#include "doors.h"
#include "collision.h"
#include "globals.h"
#include <sys/types.h>

Door::Door() {
    open = false;
};
Door::~Door() {};

void Door::set_key_type(std::string ktype) {

    if (ktype == "BLUE") {
        key_type = DOORKEY_BLUE;
    } else if (ktype == "YELLOW") {
        key_type = DOORKEY_YELLOW;
    } else if (ktype == "RED") {
        key_type = DOORKEY_RED;
    } else if (ktype == "NOKEY") {
        key_type = DOORKEY_NONE;
    }
}

void Door::update(const Collider &player, DOORKEY_TYPE *player_key) {

    if (finished_oppening) {
        return;
    }

    BoundingBox player_bb = {
        {player.pos.x - player.size.x / 2, player.pos.y - player.size.y / 2, player.pos.z - player.size.z / 2},
        {player.pos.x + player.size.x / 2, player.pos.y + player.size.y / 2, player.pos.z + player.size.z / 2},
    };

    BoundingBox trigger_bb = {
        {open_trigger.pos.x - open_trigger.size.x / 2,
         open_trigger.pos.y - open_trigger.size.y / 2,
         open_trigger.pos.z - open_trigger.size.z / 2},

        {open_trigger.pos.x + open_trigger.size.x / 2,
         open_trigger.pos.y + open_trigger.size.y / 2,
         open_trigger.pos.z + open_trigger.size.z / 2},

    };

    if (!open) {

        if (CheckCollisionBoxes(player_bb, trigger_bb)) {

            if (key_type == *player_key || key_type == DOORKEY_NONE) {

                open = true;
                PlaySound(g_sounds.door_open);
            }
        }
    }

    if (open) {

        float t = 0.05;

        collider_a.pos = Vector3Lerp(collider_a.pos, open_pos.pos_a, t);
        collider_b.pos = Vector3Lerp(collider_b.pos, open_pos.pos_b, t);

        if (Vector3Distance(collider_a.pos, open_pos.pos_a) < 0.5) {

            finished_oppening = true;
        }
    }
}

void Door::draw() const {

    collider_a.draw();
    collider_b.draw();

#ifdef DEBUG

    DrawCubeWiresV(collider_a.pos, {6, 6, 6}, YELLOW);
    DrawCubeWiresV(collider_b.pos, {6, 6, 6}, YELLOW);

    DrawSphere(open_pos.pos_a, 4, BLUE);
    DrawSphere(open_pos.pos_b, 4, BLUE);

    DrawCubeWiresV(open_trigger.pos, open_trigger.size, ORANGE);

#endif // DEBUG
}

Color door_get_color_from_keytype(std::string key_type) {

    Color ret = WHITE;

    if (key_type == "BLUE") {
        ret = BLUE;
    }

    if (key_type == "YELLOW") {
        ret = YELLOW;
    }

    if (key_type == "RED") {
        ret = RED;
    }

    return ret;
}

const char *get_key_as_cstr(DOORKEY_TYPE key_type) {

    static const char *key_names[] = {
        "KEY_BLUE",
        "KEY_YELLOW",
        "KEY_RED",
        "KEY_NONE",
    };

    if (key_type >= DOORKEY_BLUE && key_type < DOORKEY_NONE) {

        return key_names[key_type];
    }

    return "UNKNOWN_KEY";
}
