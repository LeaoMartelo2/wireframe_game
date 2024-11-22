#include "player.h"
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdbool.h>

int main(void) {

    InitWindow(1366, 768, "Boomer Shooter");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();
    SetTargetFPS(60);

    DisableCursor();

    Player player = {0};
    player.camera = (Camera3D){0};
    player.camera.position = (Vector3){0, 0, 0};
    player.camera.target = Vector3Zero();
    player.camera.up = (Vector3){0, 1, 0};
    player.camera.fovy = 90.0f;
    player.camera.projection = CAMERA_PERSPECTIVE;
    player.postition = (Vector3){10, 50, 10};
    player.turn_A = false;
    player.turn_D = false;
    player.move_speed = 500;
    player.cam_rot_speed = 0.1f;
    player.cam_rol_scale = 0.03f;
    player.gravity = -150.0f;
    /*player.gravity = 0;*/
    player.bounding_box_size = (Vector3){5, 15, 5};
    player.forward_velocity = 0;
    player.sideways_velocity = 0;
    player.vertical_velocity = player.gravity;
    player.is_grounded = false;
    player.viewmodel_pos = Vector3Zero();

    Ray ray = {0};

    Vector3 cube_pos = {0, 15, 0};

    float cube_size = 10.0f;
    Mesh cube_mesh = GenMeshCube(cube_size, cube_size, cube_size);
    Model cube_model = LoadModelFromMesh(cube_mesh);

    Vector3 floor_size = {2000, 10, 2000};
    Mesh floor_mesh = GenMeshCube(floor_size.x, floor_size.y, floor_size.z);
    Model floor_model = LoadModelFromMesh(floor_mesh);
    Vector3 floor_pos = {0, 0, 0};

    Vector3 wall_size = {300, 300, 10};
    Mesh wall_mesh = GenMeshCube(wall_size.x, wall_size.y, wall_size.z);
    Model wall_model = LoadModelFromMesh(wall_mesh);
    Vector3 wall_pos = {200, 155, 200};

    double rot = 0;

    Texture2D billboard = LoadTexture("billboard.png");
    Vector3 billboard_pos = cube_pos;

    billboard_pos.y += 20;

    Model shotgun = LoadModel("low_poly_shotgun/scene.gltf");

    while (!WindowShouldClose()) {

        move_cam(&player);
        update_player(&player);

        rot++;

        BeginDrawing();
        {
            ClearBackground(BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ray = GetScreenToWorldRay((Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0 - 2},
                                          player.camera);
            }

            BeginMode3D(player.camera);
            {

                DrawModel(floor_model, floor_pos, 1, GetColor(0x181818FF));

                DrawModel(wall_model, wall_pos, 1, DARKGRAY);
                DrawModelWires(wall_model, wall_pos, 1, RED);

                DrawModelEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLACK);
                DrawModelWiresEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLUE);

                DrawBillboard(player.camera, billboard, billboard_pos, 10, WHITE);

                DrawRay(ray, YELLOW);

                draw_viewmodel(&player, shotgun);

                DrawBoundingBox(player.bounding_box, GREEN);
            }
            EndMode3D();

            DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1, GREEN);
            DrawText(TextFormat("x:%f\ny:%f\nz:%f", player.postition.x, player.postition.y, player.postition.z),
                     5, 5, 30, WHITE);

            /* DrawText(TextFormat("velo forward:%f\nvelo sideways:%f\nvelo vertical:%f",
                                player.forward_velocity, player.sideways_velocity, player.vertical_velocity),
                     5, 100, 30, WHITE); */

            /* DrawText(TextFormat("viewmodel:\nx:%f\ny:%f\nz:%f",
                                player.viewmodel_pos.x, player.viewmodel_pos.y, player.viewmodel_pos.z),
                     5, 300, 30, WHITE); */
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
