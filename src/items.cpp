#include "items.h"
#include "globals.h"
#include "include/lognest.h"
#include <assert.h>
#include <math.h>

const char *get_item_as_cstr(PLAYER_ITEMS item) {

    static const char *item_names[] = {
        "ITEM_SHOTGUN",
        "ITEM_AXE",
        "ITEM_CABELA",
        "UNKNOWN_ITEM",
    };

    if (item >= ITEM_SHOTGUN && item < ITEM_COUNT) {

        return item_names[item];

    } else {

        return "UNKNOWN_ITEM";
    }
}

DroppedItem::DroppedItem() {
    pos = {0, 0, 0};
}

void DroppedItem::load() {

    size = Vector3One();
    collect_trigger.size = {10, 15, 10};
    collect_trigger.pos = pos;
    collect_trigger.pos.y += 5;

    switch (type) {

    case ITEM_SHOTGUN:
        player_slot = 1;
        break;

    case ITEM_AXE:
        player_slot = 2;
        break;

    case ITEM_CABELA:
        player_slot = 3;
        break;

    default:
        player_slot = 0;
        break;
    }
}

int DroppedItem::update(Vector3 player_pos, Vector3 player_size) {

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
        ret = true;
    }

    double time = GetTime();

    pos.y += sinf(time * 1.5) * 0.05;

    return ret;
}

void DroppedItem::draw() {

    rot += 1;

    switch (type) {

    case ITEM_SHOTGUN:
        DrawModelEx(g_assets.shotgun, pos, {1, 1, 1}, rot, {10, 10, 10}, FILL_COLOR);
        DrawModelWiresEx(g_assets.shotgun, pos, {1, 1, 1}, rot, {10, 10, 10}, WHITE);
        break;

    case ITEM_AXE:
        DrawModelEx(g_assets.axe, Vector3Add(pos, {0, 3.5, 0}), {1, 1, 1}, rot, {10, 10, 10}, FILL_COLOR);
        DrawModelWiresEx(g_assets.axe, Vector3Add(pos, {0, 3.5, 0}), {1, 1, 1}, rot, {10, 10, 10}, WHITE);
        break;

    case ITEM_CABELA:
        DrawModelEx(g_assets.cabela, pos, {0, 1, 0}, rot, {10, 10, 10}, GRAY);
        DrawModelWiresEx(g_assets.cabela, pos, {0, 1, 0}, rot, {10, 10, 10}, RED);
        break;

    case ITEM_EMPTY:
        lognest_error("Unreachable case ITEM_EMPTY %s:%s.", __FILE__, __LINE__);
        assert(0);
        break;

    case ITEM_COUNT:
        lognest_error("Unreachable case ITEM_COUNT %s:%s.", __FILE__, __LINE__);
        assert(0);
        break;
    }

#ifdef DEBUG

    DrawCubeWiresV(collect_trigger.pos, collect_trigger.size, ORANGE);

#endif // DEBUG
}

EmptyItem::EmptyItem() { return; }

// Hello guys, so this is a tutorial on how to make ths compiler shut the fuck up
void EmptyItem::update(GenericPlayerData_share data [[maybe_unused]]) { return; }
void EmptyItem::draw(GenericPlayerData_share data [[maybe_unused]]) { return; }
// thanks for comming to my tutorial please linker and subtract

void EmptyItem::play_equip_animation() { return; }

Shotgun::Shotgun() {
    pos = Vector3Zero();
}

void Shotgun::update(GenericPlayerData_share data) {

    timer_update(&equip_time);

    Vector3 local_forward = Vector3Add(data.forward, data.player_pos);
    local_forward = Vector3Add(local_forward, data.right);

    local_forward.y = data.player_pos.y + 6.5f;

    float velo = Vector3Length(Vector3Normalize(data.velocity));

    local_forward.y += sinf(GetTime() * 3.5) * 0.1 * velo * data.input_forward;

    pos = local_forward;
}

void Shotgun::draw(GenericPlayerData_share data) {

    rlPushMatrix();
    {
        rlTranslatef(pos.x,
                     pos.y - equip_time.life_time,
                     pos.z);

        Vector3 direction = Vector3Subtract(data.camera_pos, pos);
        direction = Vector3Normalize(direction);

        float yaw = atan2f(direction.x, direction.z);

        Matrix rotation = MatrixRotateY(yaw + 3.5f -
                                        equip_time.life_time +
                                        Normalize(data.input_sideways, -1.5f, 1.5f));

        rlMultMatrixf(MatrixToFloat(rotation));
        rlScalef(1, 1, 1);
        DrawModel(g_assets.shotgun, Vector3Zero(), 1, FILL_COLOR);
        DrawModelWires(g_assets.shotgun, Vector3Zero(), 1, WHITE);
    }
    rlPopMatrix();
}

void Shotgun::play_equip_animation() {

    timer_start(&equip_time, 1.5);

    PlaySound(g_sounds.item_shotgun_reload);
}

Axe::Axe() {
    pos = Vector3Zero();
}

void Axe::update(GenericPlayerData_share data) {

    timer_update(&equip_time);

    Vector3 new_pos = Vector3Add(data.forward, data.player_pos);
    new_pos = Vector3Add(new_pos, data.right);

    new_pos.y = data.player_pos.y + 6.6f;

    new_pos.y += sinf(GetTime() * 1.5) * 0.1;

    float velo = Vector3Length(Vector3Normalize(data.velocity));

    new_pos.y += sinf(GetTime() * 3.5) * 0.1 * velo * data.input_forward;

    pos = new_pos;
}

void Axe::draw(GenericPlayerData_share data) {

    rlPushMatrix();
    {
        rlTranslatef(pos.x, pos.y - equip_time.life_time, pos.z);

        Vector3 direction = Vector3Subtract(data.camera_pos, pos);
        direction = Vector3Normalize(direction);

        float yaw = atan2f(direction.x, direction.z);

        Vector3 camera_forward = {direction.x, 0, direction.z};
        camera_forward = Vector3Normalize(camera_forward);

        float inclination = 15 * (PI / 180);

        Matrix rotation = MatrixRotateY(yaw + (PI * 1.45) +
                                        equip_time.life_time +
                                        Normalize(data.input_forward, -5, 5));

        Matrix inclined_rot = MatrixRotate(data.right, inclination);

        Matrix final_rot = MatrixMultiply(rotation, inclined_rot);

        rlMultMatrixf(MatrixToFloat(final_rot));
        rlScalef(1, 1, 1);
        DrawModel(g_assets.axe, Vector3Zero(), 1, BLACK);
        DrawModelWires(g_assets.axe, Vector3Zero(), 1, WHITE);
    }
    rlPopMatrix();
}

void Axe::play_equip_animation() {

    timer_start(&equip_time, 1.5);

    int index = GetRandomValue(0, 2);

    PlaySound(g_sounds.item_axe_equip[index]);
}

Cabela::Cabela() {
    pos = Vector3Zero();
}

void Cabela::update(GenericPlayerData_share data) {

    timer_update(&equip_time);

    Vector3 new_pos = Vector3Add(data.forward, data.player_pos);
    new_pos = Vector3Add(new_pos, data.right);

    new_pos.y = data.player_pos.y + 6.5f;

    float velo = Vector3Length(Vector3Normalize(data.velocity));

    new_pos.y += sinf(GetTime() * 3.5) * 0.1 * velo * data.input_forward;

    pos = new_pos;
}

void Cabela::draw(GenericPlayerData_share data) {

    rlPushMatrix();
    {
        rlTranslatef(pos.x, pos.y - (equip_time.life_time * 2), pos.z);

        Vector3 direction = Vector3Subtract(data.camera_pos, pos);
        direction = Vector3Normalize(direction);

        float yaw = atan2f(direction.x, direction.z);

        Matrix rotation = MatrixRotateY(yaw + PI + (equip_time.life_time * 5));

        rlMultMatrixf(MatrixToFloat(rotation));
        rlScalef(1, 1, 1);
        DrawModel(g_assets.cabela, Vector3Zero(), 1, GRAY);
        DrawModelWires(g_assets.cabela, Vector3Zero(), 1, RED);
    }
    rlPopMatrix();
}

void Cabela::play_equip_animation() {

    timer_start(&equip_time, 1.5);

    PlaySound(g_sounds.item_cabela_interact);

    return;
}
