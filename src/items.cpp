#include "items.h"
#include "globals.h"
#include <cmath>

Shotgun::Shotgun() {
    pos = Vector3Zero();
};

void Shotgun::update(Vector3 forward, Vector3 right, Vector3 player_pos) {

    forward = Vector3Add(forward, player_pos);
    forward = Vector3Add(forward, right);

    forward.y = player_pos.y + 6.5f;

    pos = forward;
};

void Shotgun::draw(Vector3 camera_pos, float input_sideways) {

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

    Vector3 new_pos = Vector3Add(forward, player_pos);
    new_pos = Vector3Add(new_pos, right);

    new_pos.y = player_pos.y + 6.6f;

    new_pos.y += sinf(GetTime() * 1.5) * 0.1;

    pos = new_pos;
};

void Axe::draw(Vector3 camera_pos, float input_forward, Vector3 player_right) {

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
