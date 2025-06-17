#include "doors.h"
#include "collision.h"
#include "globals.h"
#include "items.h"
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
    } else if (ktype == "GREEN") {
        key_type = DOORKEY_GREEN;
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

                if (key_type == *player_key) {
                    *player_key = DOORKEY_NONE;
                }
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

    if (key_type == "GREEN") {
        ret = GREEN;
    }

    return ret;
}

Color get_keycolor_from_keytipe_enum(DOORKEY_TYPE key) {

    Color ret = {};

    switch (key) {
    case DOORKEY_BLUE:
        ret = BLUE;
        break;

    case DOORKEY_YELLOW:
        ret = YELLOW;
        break;

    case DOORKEY_GREEN:
        ret = GREEN;
        break;

    case DOORKEY_NONE:
        ret = WHITE;
        break;

    default:
        ret = WHITE;
    }

    return ret;
}

DOORKEY_TYPE get_keytpe_from_string(const std::string key_type) {

    DOORKEY_TYPE ret = DOORKEY_NONE;

    if (key_type == "BLUE") {
        ret = DOORKEY_BLUE;
    }

    if (key_type == "YELLOW") {
        ret = DOORKEY_YELLOW;
    }

    if (key_type == "GREEN") {
        ret = DOORKEY_GREEN;
    }

    return ret;
}

const char *get_key_as_cstr(DOORKEY_TYPE key_type) {

    static const char *key_names[] = {
        "KEY_BLUE",
        "KEY_YELLOW",
        "KEY_GREEN",
        "KEY_NONE",
    };

    if (key_type >= DOORKEY_BLUE && key_type <= DOORKEY_NONE) {

        return key_names[key_type];
    }

    return "UNKNOWN_KEY";
}

DOORKEY_TYPE get_keytype_from_color(Color key_color) {

    DOORKEY_TYPE ret;

    if (ColorIsEqual(key_color, BLUE)) {
        ret = DOORKEY_BLUE;

    } else if (ColorIsEqual(key_color, YELLOW)) {
        ret = DOORKEY_YELLOW;

    } else if (ColorIsEqual(key_color, GREEN)) {
        ret = DOORKEY_GREEN;
    } else {
        ret = DOORKEY_NONE;
    }

    return ret;
}

DroppedKey::DroppedKey() { return; };

void DroppedKey::load() {
    size = {50, 50, 50};
    collect_trigger.size = {10, 10, 10};
    collect_trigger.pos = pos;
    collect_trigger.pos.y += 2.5;

    key_color = get_keycolor_from_keytipe_enum(type);
}

int DroppedKey::update(Vector3 player_pos, Vector3 player_size, DOORKEY_TYPE *player_key) {

    int ret = false;

    BoundingBox collect_bb = {
        {collect_trigger.pos.x - collect_trigger.size.x / 2,
         collect_trigger.pos.y - collect_trigger.size.y / 2,
         collect_trigger.pos.z - collect_trigger.size.z / 2},

        {collect_trigger.pos.x + collect_trigger.size.x / 2,
         collect_trigger.pos.y + collect_trigger.size.y / 2,
         collect_trigger.pos.z + collect_trigger.size.z / 2},
    };

    BoundingBox player_bb = {
        {player_pos.x - player_size.x / 2, player_pos.y - player_size.y / 2, player_pos.z - player_size.z / 2},
        {player_pos.x + player_size.x / 2, player_pos.y + player_size.y / 2, player_pos.z + player_size.z / 2},
    };

    if (CheckCollisionBoxes(collect_bb, player_bb)) {

        if (*player_key == DOORKEY_NONE) {

            ret = true;

            *player_key = type;

            int pickup_index = GetRandomValue(0, 2);
            PlaySound(g_sounds.item_pickup_sound[pickup_index]);
        }
    }

    double time = GetTime();

    pos.y += sinf(time * 1.5) * 0.05;

    return ret;
}

void DroppedKey::draw() {

    DrawModelEx(g_assets.key, pos, {0, 1, 0}, rot, {50, 50, 50}, FILL_COLOR);
    DrawModelWiresEx(g_assets.key, pos, {0, 1, 0}, rot, {50, 50, 50}, key_color);

    rot += -1.5;

#ifdef DEBUG

    DrawCubeWiresV(collect_trigger.pos, collect_trigger.size, ORANGE);

#endif // DEBUG
}
