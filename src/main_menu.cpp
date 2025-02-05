#include "globals.h"
#include "gui.h"
#include "include/lognest.h"
#include "misc.h"
#include "scene.h"
#include "scene_manager.h"

#define MENU_MODEL "assets/models/low_poly_shotgun/shotgun.gltf"
#define MENU_MUSIC "assets/music/menu.mp3"
#define MENU_CLICK "assets/sounds/snd_button.wav"
#define MENU_CONFIG "assets/sprites/menu_settings.png"
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
    rot += GetFrameTime() * 60;
}

MainMenu::MainMenu() {

    lognest_trace("[Menu] Loading Main menu data.");

    mm_camera = {0};
    mm_camera.position = Vector3Zero();
    mm_camera.target = Vector3{10, 0, 0};
    mm_camera.up = {0, 1, 0};
    mm_camera.fovy = 90.0f;
    mm_camera.projection = CAMERA_PERSPECTIVE;

    lognest_debug(" ┗>[Menu] Camera created.");

    menu_model = LoadModel(MENU_MODEL);
    menu_click = LoadSound(MENU_CLICK);
    menu_music = LoadMusicStream(MENU_MUSIC);
    mmenu_settings = LoadTexture(MENU_CONFIG);

    lognest_debug(" ┗>[Menu] Models and sounds loaded.");
}

MainMenu::~MainMenu() {
}

void MainMenu::start() {
    EnableCursor();

    PlayMusicStream(menu_music);
    lognest_debug(" ┗>[Menu] Started playing music stream.");
}

void MainMenu::end() {
    UnloadMusicStream(menu_music);
    UnloadModel(menu_model);
    UnloadSound(menu_click);

    UnloadTexture(mmenu_settings);

    lognest_debug("[Menu] Unloaded models/sounds.");
}

void MainMenu::update() {

    static bool settings_menu = false;
    static bool test_cfg = false;

    static const gui_color_scheme mmenu_buttons = {
        .default_color = GetColor(0x181818FF),
        .hoovered_color = DARKGRAY,
        .pressed_color = GRAY,
        .text_color = WHITE,
    };

    static const gui_color_scheme settings_buttons = {
        .default_color = DARKGRAY,
        .hoovered_color = GRAY,
        .pressed_color = ColorBrightness(GRAY, -0.5),
        .text_color = WHITE,
    };

    static gui_button_t fullscreen_setting = {
        .bounds = {
            .x = 50,
            .y = 160,
            .width = 100,
            .height = 50,
        },
        .button_style = GUI_SQUARE,
        .font_size = 20,
        .colors = &settings_buttons,
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

    Rectangle settings_bounds = {
        .x = GetScreenWidth() - 100.0f,
        .y = GetScreenHeight() - 100.0f,
        .width = (float)mmenu_settings.width * 0.75f,
        .height = (float)mmenu_settings.height * 0.75f,
    };

    static gui_panel settings_pannel = {
        .pos = {GetScreenWidth() / 2.0f - 500, GetScreenHeight() / 2.0f - 300},
        .size = {1000, 600},
        .color = DARKGRAY,
        .toggle = &settings_menu,
        .exit_pos = {940, 30},
        .exit_size = {50, 50},
    };

    UpdateMusicStream(menu_music);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(mm_camera);
        {
            if (!settings_menu) {
                main_menu_shotgun(&menu_model);
            }
        }
        EndMode3D();

        draw_tittle();

        if (!settings_menu) {
            if (gui_button_ex(&play_button, "Play")) {
                PlaySound(menu_click);
                parent->swap_scene(SCENE_LEVEL_TEST1);
            }

            if (gui_button_ex(&quit_button, "Quit")) {
                PlaySound(menu_click);
                close_application = true;
            }
        }

        {
            if (gui_button(&settings_bounds, GUI_ROUNDED, "", 50, &gui_transparent)) {
                PlaySound(menu_click);
                settings_menu = !settings_menu;
            }

            /*DrawTextureEx(mmenu_settings, Vector2{GetScreenWidth() - 100.0f, GetScreenHeight() - 100.0f},*/
            /*0, 0.75, WHITE);*/

            DrawTextureEx(mmenu_settings, {settings_bounds.x, settings_bounds.y}, 0, 0.75, WHITE);
        }

        if (settings_menu) {
            draw_panel(&settings_pannel);
            draw_text_in_pannel_space(&settings_pannel, "Settings", 50, {30, 30});

            draw_text_in_pannel_space(&settings_pannel, "FullScreen", 30, {50, 130});
            if (gui_button_on_pannel(&settings_pannel, &fullscreen_setting,
                                     TextFormat("%s", bool_to_string(g_settings.fullscreen)))) {
                PlaySound(menu_click);
                g_settings.fullscreen = !g_settings.fullscreen;
                ToggleFullscreen();
                MaximizeWindow();
            }
        }
    }
    EndDrawing();
}
