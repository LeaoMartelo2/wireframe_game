#include "items.h"
#include "globals.h"
#include "include/lognest.h"
#include <assert.h>

DroppedItem::DroppedItem() {};

int DroppedItem::update(Vector3 player_pos, Vector3 player_size) {

    if (collected) {
        return -1;
    }

    int ret = -1;

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
        collected = true;
        ret = this->type;
    }

    pos.y += sinf(GetTime() * 1.5) * 0.1;

    return ret;
};

void DroppedItem::draw() {

    if (collected) {
        return;
    }

    static long double rot = 0;
    rot++;

    switch (type) {

    case ITEM_SHOTGUN:
        DrawModelEx(g_assets.shotgun, pos, {1, 1, 1}, rot, {10, 10, 10}, FILL_COLOR);
        DrawModelWiresEx(g_assets.shotgun, pos, {1, 1, 1}, rot, {10, 10, 10}, WHITE);
        break;

    case ITEM_AXE:
        DrawModelEx(g_assets.axe, pos, {1, 1, 1}, rot, {10, 10, 10}, FILL_COLOR);
        DrawModelWiresEx(g_assets.axe, pos, {1, 1, 1}, rot, {10, 10, 10}, WHITE);
        break;

    case ITEM_CABELA:
        DrawModelEx(g_assets.cabela, pos, {0, 1, 0}, rot, {10, 10, 10}, GRAY);
        DrawModelWiresEx(g_assets.cabela, pos, {0, 1, 0}, rot, {10, 10, 10}, RED);
        break;

    case ITEM_COUNT:
        lognest_error("Unreachable.");
        assert(1);
    }

#ifdef DEBUG

    DrawCubeWiresV(collect_trigger.pos, collect_trigger.size, ORANGE);

#endif // DEBUG
};

Shotgun::Shotgun() {
    pos = Vector3Zero();
};

void Shotgun::update(Vector3 forward, Vector3 right, Vector3 player_pos) {

    if (!enabled) {
        return;
    }

    forward = Vector3Add(forward, player_pos);
    forward = Vector3Add(forward, right);

    forward.y = player_pos.y + 6.5f;

    pos = forward;
};

void Shotgun::draw(Vector3 camera_pos, float input_sideways) {

    if (!enabled) {
        return;
    }

    rlPushMatrix();
    {
        rlTranslatef(pos.x, pos.y, pos.z);

        Vector3 direction = Vector3Subtract(camera_pos, pos);
        direction = Vector3Normalize(direction);

        float yaw = atan2f(direction.x, direction.z);

        Matrix rotation = MatrixRotateY(yaw + 3.5f + Normalize(input_sideways, -1.5f, 1.5));

        rlMultMatrixf(MatrixToFloat(rotation));
        rlScalef(1, 1, 1);
        DrawModel(g_assets.shotgun, Vector3Zero(), 1, FILL_COLOR);
        DrawModelWires(g_assets.shotgun, Vector3Zero(), 1, WHITE);
    }
    rlPopMatrix();
}

Axe::Axe() {
    pos = Vector3Zero();
};

void Axe::update(Vector3 forward, Vector3 right, Vector3 player_pos) {

    if (!enabled) {
        return;
    }

    Vector3 new_pos = Vector3Add(forward, player_pos);
    new_pos = Vector3Add(new_pos, right);

    new_pos.y = player_pos.y + 6.6f;

    new_pos.y += sinf(GetTime() * 1.5) * 0.1;

    pos = new_pos;
};

void Axe::draw(Vector3 camera_pos, float input_forward, Vector3 player_right) {

    if (!enabled) {
        return;
    }

    rlPushMatrix();
    {
        rlTranslatef(pos.x, pos.y, pos.z);

        Vector3 direction = Vector3Subtract(camera_pos, pos);
        direction = Vector3Normalize(direction);

        float yaw = atan2f(direction.x, direction.z);

        Vector3 camera_forward = {direction.x, 0, direction.z};
        camera_forward = Vector3Normalize(camera_forward);

        float inclination = 15 * (PI / 180);

        Matrix rotation = MatrixRotateY(yaw + (PI * 1.45) + Normalize(input_forward, -5, 5));
        Matrix inclined_rot = MatrixRotate(player_right, inclination);

        Matrix final_rot = MatrixMultiply(rotation, inclined_rot);

        rlMultMatrixf(MatrixToFloat(final_rot));
        rlScalef(1, 1, 1);
        DrawModel(g_assets.axe, Vector3Zero(), 1, BLACK);
        DrawModelWires(g_assets.axe, Vector3Zero(), 1, WHITE);
    }
    rlPopMatrix();
}

Cabela::Cabela() {
    pos = Vector3Zero();
}

void Cabela::update(Vector3 forward, Vector3 right, Vector3 player_pos) {

    if (!enabled) {
        return;
    }

    forward = Vector3Add(forward, player_pos);
    forward = Vector3Add(forward, right);

    forward.y = player_pos.y + 6.5f;

    pos = forward;
}

void Cabela::draw(Vector3 camera_pos) {

    if (!enabled) {
        return;
    }

    rlPushMatrix();
    {
        rlTranslatef(pos.x, pos.y, pos.z);

        Vector3 direction = Vector3Subtract(camera_pos, pos);
        direction = Vector3Normalize(direction);

        float yaw = atan2f(direction.x, direction.z);

        Matrix rotation = MatrixRotateY(yaw + PI);

        rlMultMatrixf(MatrixToFloat(rotation));
        rlScalef(1, 1, 1);
        DrawModel(g_assets.cabela, Vector3Zero(), 1, GRAY);
        DrawModelWires(g_assets.cabela, Vector3Zero(), 1, RED);
    }
    rlPopMatrix();
}
