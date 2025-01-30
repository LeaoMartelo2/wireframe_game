#include "scene.h"

#define MENU_MODEL "assets/models/low_poly_shotgun/shotgun.gltf"
#define FILL_COLOR GetColor(0x181818FF)

void main_menu_shotgun(Model *menu_model) {

    Vector3 pos = {1, 0, 0.75};
    Vector3 rot_axis = {0, 1, 0.75};

    static double rot = 0;
    DrawModelEx(*menu_model, pos, rot_axis, rot, Vector3One(), FILL_COLOR);
    DrawModelWiresEx(*menu_model, pos, rot_axis, rot, Vector3One(), WHITE);
    ++rot;
}

MainMenu::MainMenu() {

    mm_camera = *new Camera3D;
    mm_camera.position = Vector3Zero();
    mm_camera.target = Vector3{10, 0, 0};
    mm_camera.up = {0, 1, 0};
    mm_camera.fovy = 90.0f;
    mm_camera.projection = CAMERA_PERSPECTIVE;

    menu_model = LoadModel(MENU_MODEL);
}

MainMenu::~MainMenu() {
}

void MainMenu::start() {
    EnableCursor();
}

void MainMenu::update() {

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(mm_camera);
        {
            main_menu_shotgun(&menu_model);
        }
        EndMode3D();

        DrawText("Wireframe Game", 50, 50, 70, WHITE);
    }
    EndDrawing();
}
