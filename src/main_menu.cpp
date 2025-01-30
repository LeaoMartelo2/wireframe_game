#include "scene.h"

#define MENU_MODEL "assets/models/low_poly_shotgun/shotgun.gltf"
#define MENU_MUSIC "assets/music/menu.mp3"
#define MENU_CLICK "assets/sounds/snd_button.wav"
#define FILL_COLOR GetColor(0x181818FF)

void draw_tittle(void) {

    static Sound menu_click = LoadSound(MENU_CLICK);

    static float x = 50;
    static float y = 50;
    static float size = 70;

    const char *tittle = "Wireframe Game";

    static int frames_counter = 0;
    frames_counter += 5;

    if (frames_counter < 200) {
        if (frames_counter % 4 == 0) {
            PlaySound(menu_click);
        }
    }

    /*DrawText(tittle, x, y, size, WHITE);*/
    DrawText(TextSubtext(tittle, 0, frames_counter / 10), x, y, size, WHITE);
}

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
    menu_music = LoadMusicStream(MENU_MUSIC);

    PlayMusicStream(menu_music);
}

void MainMenu::update() {

    UpdateMusicStream(menu_music);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(mm_camera);
        {
            main_menu_shotgun(&menu_model);
        }
        EndMode3D();

        draw_tittle();
    }
    EndDrawing();
}
