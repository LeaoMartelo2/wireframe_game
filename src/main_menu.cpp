#include "globals.h"
#include "gui.h"
#include "scene.h"
#include "scene_manager.h"
#include <cstdio>

#define MENU_MODEL "assets/models/low_poly_shotgun/shotgun.gltf"
#define MENU_MUSIC "assets/music/menu.mp3"
#define MENU_CLICK "assets/sounds/snd_button.wav"
#define FILL_COLOR GetColor(0x181818FF)

void draw_tittle(void) {

    static Sound menu_click = LoadSound(MENU_CLICK);

    static const Vector2 pos = {50, 50};
    static const float size = 70;

    static const char *tittle = "Wireframe Game";

    static int frames_counter = 0;
    if (frames_counter < 200) {
        frames_counter += 5;
    }

    DrawText(TextSubtext(tittle, 0, frames_counter / 10), pos.x, pos.y, size, WHITE);

    if (frames_counter < 200) {
        if (frames_counter % 4 == 0) {
            PlaySound(menu_click);
        }
    }

    // name

    static const Vector2 name_pos = {70, 120};
    static const float name_size = 25;
    static const char *name = "By LeaoMartelo";

    static int name_framecounter = 0;
    if (name_framecounter < 800) {
        name_framecounter += 5;
    }

    DrawText(TextSubtext(name, 0, name_framecounter / 50), name_pos.x, name_pos.y, name_size, WHITE);

    if (name_framecounter < 1000) {
        if (name_framecounter % 17 == 0) {
            PlaySound(menu_click);
        }
    }
}

void main_menu_shotgun(Model *menu_model) {

    static Vector3 pos = {1, 0, 5}; // Z = 0.75
    Vector3 rot_axis = {0, 1, 0.75};

    if (pos.z > 0.75f) {
        pos.z -= 0.05f;
    }

    static double rot = 0;
    DrawModelEx(*menu_model, pos, rot_axis, rot, Vector3One(), FILL_COLOR);
    DrawModelWiresEx(*menu_model, pos, rot_axis, rot, Vector3One(), WHITE);
    ++rot;
}

MainMenu::MainMenu() {

    mm_camera = {0};
    mm_camera.position = Vector3Zero();
    mm_camera.target = Vector3{10, 0, 0};
    mm_camera.up = {0, 1, 0};
    mm_camera.fovy = 90.0f;
    mm_camera.projection = CAMERA_PERSPECTIVE;

    menu_model = LoadModel(MENU_MODEL);
    menu_click = LoadSound(MENU_CLICK);
}

MainMenu::~MainMenu() {
}

void MainMenu::start() {
    EnableCursor();
    menu_music = LoadMusicStream(MENU_MUSIC);

    PlayMusicStream(menu_music);
}

void MainMenu::end() {
    UnloadMusicStream(menu_music);
    UnloadModel(menu_model);
    UnloadSound(menu_click);
}

void MainMenu::update() {

    UpdateMusicStream(menu_music);

    static const gui_color_scheme mmenu_buttons = {
        .default_color = GetColor(0x181818FF),
        .hoovered_color = DARKGRAY,
        .pressed_color = GRAY,
        .text_color = WHITE,
    };

    static gui_button_t play_button = {
        .bounds = {
            .x = GetScreenWidth() / 16.0f,
            .y = GetScreenHeight() / 2.0f,
            .width = 220,
            .height = 100,
        },
        .button_style = GUI_SQUARE,
        .font_size = 50,
        .colors = &mmenu_buttons,
    };

    static gui_button_t quit_button = {
        .bounds = {
            .x = GetScreenWidth() / 16.0f,
            .y = play_button.bounds.y + 105,
            .width = 220,
            .height = 100,
        },
        .button_style = GUI_SQUARE,
        .font_size = 50,
        .colors = &mmenu_buttons,
    };

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(mm_camera);
        {
            main_menu_shotgun(&menu_model);
        }
        EndMode3D();

        draw_tittle();

        if (gui_button_ex(&play_button, "Play")) {
            PlaySound(menu_click);
            parent->swap_scene(SCENE_LEVEL_TEST1);
        }

        if (gui_button_ex(&quit_button, "Quit")) {
            PlaySound(menu_click);
            close_application = true;
        }
    }
    EndDrawing();
}
