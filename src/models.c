#include "models.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define GROUND_PLATES_COUNT 2

void load_map(Solid *solids, int *count, Ground *ground_geometry) {

    int solid_count = 0;

    Ground floor = {0};
    floor.size = (Vector3){2000, 1, 2000};
    floor.mesh = GenMeshCube(floor.size.x, floor.size.y, floor.size.z);
    floor.model = LoadModelFromMesh(floor.mesh);
    floor.pos = (Vector3){0, 0, 0};

    ground_geometry[0] = floor;

    Solid wall;
    wall.size = (Vector3){300, 300, 10};
    wall.mesh = GenMeshCube(wall.size.x, wall.size.y, wall.size.z);
    wall.model = LoadModelFromMesh(wall.mesh);
    wall.pos = (Vector3){200, 152, 200};

    solids[0] = wall;
    solid_count++;

    *count = solid_count;

    printf("%i\n", *count);
}

void draw_map(const Solid *solids, int count, const Ground *ground_plates) {

    for (int i = 0; i < GROUND_PLATES_COUNT; i++) {
        DrawModel(ground_plates[i].model, ground_plates[i].pos, 1.0f, GetColor(0x181818FF));
    }

    for (int i = 0; i < count; i++) {

        DrawModel(solids[i].model, solids[0].pos, 1.0f, DARKGRAY);
        DrawModelWires(solids[i].model, solids[0].pos, 1.0f, RED);
    }
}
