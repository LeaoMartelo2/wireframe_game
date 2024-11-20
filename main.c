#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdbool.h>

bool turn_left = false;
bool turn_right = false;

void move_cam(Camera *camera) {
    Vector2 mouse_pos_delta = GetMouseDelta();

    float cam_mov_speed = 300.0f * GetFrameTime();
    float cam_rot_speed = 0.1f * GetFrameTime();
    float cam_rol_scale = 0.1f;

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        cam_mov_speed = 500.0f * GetFrameTime();
    }

    Vector3 forward = GetCameraForward(camera);

    CameraYaw(camera, -mouse_pos_delta.x * cam_rot_speed, false);
    CameraPitch(camera, -mouse_pos_delta.y * cam_rot_speed, false, false, false);

    if (IsKeyDown(KEY_W)) {
        CameraMoveForward(camera, cam_mov_speed, true);
    }

    if (IsKeyDown(KEY_A)) {
        CameraMoveRight(camera, -cam_mov_speed, true);

        if (turn_left == false) {
            camera->up = Vector3RotateByAxisAngle(camera->up, forward, cam_rol_scale);
        }

        turn_left = true;
    }

    if (IsKeyReleased(KEY_A)) {
        camera->up = Vector3Zero();
        camera->up.y = 1.0f;
        turn_left = false;
    }

    if (IsKeyDown(KEY_S)) {
        CameraMoveForward(camera, -cam_mov_speed, true);
    }

    if (IsKeyDown(KEY_D)) {
        CameraMoveRight(camera, cam_mov_speed, true);

        if (turn_right == false) {
            camera->up = Vector3RotateByAxisAngle(camera->up, forward, -cam_rol_scale);
        }
        turn_right = true;
    }
    if (IsKeyReleased(KEY_D)) {
        camera->up = Vector3Zero();
        camera->up.y = 1.0f;
        turn_right = false;
    }
}

int main(void) {

    InitWindow(800, 600, "Boomer Shooter");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();
    SetTargetFPS(60);

    DisableCursor();

    Camera3D camera = {0};
    camera.position = (Vector3){0, 10, 10};
    camera.target = (Vector3){0, 0, 0};
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Ray ray = {0};

    Vector3 cube_pos = {0};
    Vector2 cube_screen_pos = {0};

    float size = 10.0f;
    Mesh cube_mesh = GenMeshCube(size, size, size);
    Model cube_model = LoadModelFromMesh(cube_mesh);

    double rot = 0;

    while (!WindowShouldClose()) {

        move_cam(&camera);

        Vector3 player_pos = camera.position;
        player_pos.y = 5;

        cube_screen_pos = GetWorldToScreen(cube_pos, camera);

        rot++;

        BeginDrawing();
        {
            ClearBackground(BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ray = GetScreenToWorldRay((Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0 - 2}, camera);
            }

            BeginMode3D(camera);
            /*rlFPCameraBeginMode3D(&cam);*/
            {
                DrawCubeWires((Vector3){0, -50, 0}, 300, 10, 300, RED);
                /*DrawCubeWires(player_pos, 5, 10, 5, RED);*/
                /*DrawCubeWires((Vector3){100, 0, 100}, 10, 10, 10, BLUE);*/
                /*DrawModelEx(cube_model, cube_pos, (Vector3){10, 10, 10}, 0, Vector3One(), BLUE );*/
                /*DrawModelEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLUE);*/
                DrawModelWiresEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLUE);

                DrawRay(ray, YELLOW);
            }
            EndMode3D();

            DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1, GREEN);

            /*DrawText(TextFormat("camera up:\nx:%f\ny:%f\nz:%f", camera.up.x, camera.up.y, camera.up.z),*/
            /*5, 5, 30, RED);*/
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
