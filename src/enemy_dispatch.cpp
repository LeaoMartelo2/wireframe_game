#include "enemy_dispatch.h"

void draw_cabela(const Enemy_body_parts *cabela, const Vector3 pos, const float angle) {

    DrawModelEx(cabela->model,
                Vector3Add(pos, cabela->offset),
                ROTATION_AXIS,
                angle,
                Vector3Multiply(SCALE, cabela->custom_scale),
                GRAY);

    DrawModelWiresEx(cabela->model,
                     Vector3Add(pos, cabela->offset),
                     ROTATION_AXIS,
                     angle,
                     Vector3Multiply(SCALE, cabela->custom_scale),
                     RED);
}

void draw_shotgun(const Enemy_body_parts *shotgun, const Vector3 pos, const float angle) {

    DrawModelEx(shotgun->model,
                Vector3Add(pos, shotgun->offset),
                ROTATION_AXIS,
                angle,
                Vector3Multiply(SCALE, shotgun->custom_scale),
                GetColor(0x181818FF));

    DrawModelWiresEx(shotgun->model,
                     Vector3Add(pos, shotgun->offset),
                     ROTATION_AXIS,
                     angle,
                     Vector3Multiply(SCALE, shotgun->custom_scale),
                     WHITE);
}


void update_pos_right(Enemy_body_parts *part, Enemy *enemy){

    part->offset = Vector3Scale(enemy->get_right(), part->distance);

}


void update_pos_vec(Enemy_body_parts *part, Vector3 vec){

    part->offset = Vector3Scale(vec, part->distance);

}
