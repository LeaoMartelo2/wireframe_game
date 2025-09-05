#include "enemy_draw.h"

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
