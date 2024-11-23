#include "models.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define GROUND_PLATES_COUNT 2
#define PROP_COUNT 2

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
    wall.see_through = false;

    solids[0] = wall;
    solid_count++;

    Solid transparent_wall;

    transparent_wall.size = (Vector3){10, 300, 300};
    transparent_wall.mesh = GenMeshCube(transparent_wall.size.x, transparent_wall.size.y, transparent_wall.size.z);
    transparent_wall.model = LoadModelFromMesh(transparent_wall.mesh);
    transparent_wall.pos = (Vector3){600, 152, 200};
    transparent_wall.see_through = true;

    solids[1] = transparent_wall;
    solid_count++;

    *count = solid_count;

    printf("%i\n", *count);
}

void draw_map(const Solid *solids, int count, const Ground *ground_plates) {

    for (int i = 0; i < GROUND_PLATES_COUNT; i++) {

        DrawModel(ground_plates[i].model, ground_plates[i].pos, 1.0f, GetColor(0x181818FF));
        DrawModelWires(ground_plates[i].model, ground_plates[i].pos, 1.0f, RED);
    }

    for (int i = 0; i < count; i++) {

        if (!solids[i].see_through) {
            DrawModel(solids[i].model, solids[i].pos, 1.0f, GetColor(0x181818FF));
            DrawModelWires(solids[i].model, solids[i].pos, 1.0f, RED);
        }

        if (solids[i].see_through) {
            DrawModel(solids[i].model, solids[i].pos, 1.0f, ColorAlpha(GetColor(0x181818FF), 0.3f));
            DrawModelWires(solids[i].model, solids[i].pos, 1.0f, RED);
        }
    }
}

void load_props(Prop *props) {

    Prop cube = {0};
    cube.pos = (Vector3){0, 30, 0};
    cube.size = (Vector3){30, 30, 30};
    cube.mesh = GenMeshCube(cube.size.x, cube.size.y, cube.size.z);
    cube.model = LoadModelFromMesh(cube.mesh);
    cube.rotate = true;
    cube.bounce = false;

    props[0] = cube;

    Prop static_cube = {0};
    static_cube.pos = (Vector3){35, 15, 0};
    static_cube.size = (Vector3){10, 10, 10};
    static_cube.mesh = GenMeshCube(10, 10, 10);
    static_cube.model = LoadModelFromMesh(static_cube.mesh);
    static_cube.rotate = false;
    static_cube.bounce = false;

    props[1] = static_cube;
}

double rot = 0;

void draw_props(Prop *props) {

    rot++;

    for (int i = 0; i < PROP_COUNT; i++) {
        if (props[i].rotate) {

            DrawModelEx(props[i].model, props[i].pos, Vector3One(), rot, Vector3One(), BLACK);
            DrawModelWiresEx(props[i].model, props[i].pos, Vector3One(), rot, Vector3One(), BLUE);
        }

        if (!props[i].rotate) {
            DrawModelEx(props[i].model, props[i].pos, Vector3One(), 0, Vector3One(), BLACK);
            DrawModelWiresEx(props[i].model, props[i].pos, Vector3One(), 0, Vector3One(), BLUE);
        }
    }
}
