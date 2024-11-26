#include "models.h"
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include <math.h>

#define GROUND_PLATES_COUNT 10
#define PROP_COUNT 10

void load_map(Solid *solids, int *count, Ground *ground_geometry) {

#include "map/map_geometry.map"
}

void draw_map(const Solid *solids, int count, const Ground *ground_plates) {

    for (int i = 0; i < GROUND_PLATES_COUNT; i++) {

        DrawModel(ground_plates[i].model, ground_plates[i].pos, 1.0f, GetColor(0x181818FF));
        DrawModelWires(ground_plates[i].model, ground_plates[i].pos, 1.0f, RED);
    }

    for (int i = 0; i < count; i++) {

        if (!solids[i].see_through) {
            DrawModel(solids[i].model, solids[i].pos, 1.0f, DARKGRAY);
            DrawModelWires(solids[i].model, solids[i].pos, 1.0f, RED);
        }

        if (solids[i].see_through) {
            DrawModel(solids[i].model, solids[i].pos, 1.0f, ColorAlpha(GetColor(0x181818FF), 0.8f));
            DrawModelWires(solids[i].model, solids[i].pos, 1.0f, RED);
        }
    }
}

void load_props(Prop *props) {

#include "map/map_props.map"
}

double rot = 0;

void draw_props(Prop *props) {

    rot++;
    float time = GetTime();

    for (int i = 0; i < PROP_COUNT; i++) {

        Vector3 pos_internal = props[i].pos;

        if (props[i].bounce) {
            pos_internal.y += sinf(time * 6) * 2.0f;
        }

        if (props[i].rotate) {

            DrawModelEx(props[i].model, pos_internal, Vector3One(), rot, Vector3One(), BLACK);
            DrawModelWiresEx(props[i].model, pos_internal, Vector3One(), rot, Vector3One(), BLUE);
        }

        if (!props[i].rotate) {
            DrawModelEx(props[i].model, pos_internal, Vector3One(), 0, Vector3One(), BLACK);
            DrawModelWiresEx(props[i].model, pos_internal, Vector3One(), 0, Vector3One(), BLUE);
        }
    }
}
