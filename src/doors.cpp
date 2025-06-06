#include "doors.h"
#include "collision.h"
#include "globals.h"

Door::Door() {
    open = false;
};
Door::~Door() {};

void Door::update(const Collider &player) {

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
            open = true;
            PlaySound(g_sounds.door_open);
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
