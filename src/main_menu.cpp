#include "globals.h"
#include "gui.h"
#include "include/lognest.h"
#include "misc.h"
#include "scene.h"
#include "scene_manager.h"

#define MENU_MODEL "assets/models/low_poly_shotgun/shotgun.gltf"
#define MENU_MUSIC "assets/music/menu.mp3"
#define MENU_CONFIG "assets/sprites/menu_settings.png"
#define FILL_COLOR GetColor(0x181818FF)

#define text_settings(text)                                                  \
    do {                                                                     \
        draw_text_in_pannel_space(&settings_pannel, text, 30, {50, offset}); \
        offset += 100;                                                       \
    } while (0);

#define bool2string(boolean) TextFormat("%s", bool_to_string_c(boolean))

#define bool2enabledisable(boolean) TextFormat("%s", enabled_disabled_cstr(boolean))

void draw_tittle(void) {

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
            PlaySound(g_sounds.generic_click);
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
            PlaySound(g_sounds.generic_click);
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

    mm_camera = {};
    mm_camera.position = Vector3Zero();
    mm_camera.target = Vector3{10, 0, 0};
    mm_camera.up = {0, 1, 0};
    mm_camera.fovy = 90.0f;
    mm_camera.projection = CAMERA_PERSPECTIVE;

    lognest_debug(" ┗>[Menu] Camera created.");

    menu_model = LoadModel(MENU_MODEL);
    menu_music = LoadMusicStream(MENU_MUSIC);
    mmenu_settings = LoadTexture(MENU_CONFIG);

    lognest_debug(" ┗>[Menu] Models and sounds loaded.");
}

MainMenu::~MainMenu() {
}

void MainMenu::start() {
    g_gamestate.is_paused = true;

    PlayMusicStream(menu_music);
    lognest_debug("[Menu] Started playing music stream.");
}

void MainMenu::end() {
    UnloadMusicStream(menu_music);
    UnloadModel(menu_model);

    UnloadTexture(mmenu_settings);

    lognest_debug("[Menu] Unloaded models/sounds.");
}

void MainMenu::update() {

    static const gui_color_scheme mmenu_buttons = {
        .default_color = GetColor(0x181818FF),
        .hoovered_color = DARKGRAY,
        .pressed_color = GRAY,
        .text_color = WHITE,
    };

    // main menu buttons
    button_create(play_button, 50, &mmenu_buttons,
                  GetScreenWidth() / 16.0f, GetScreenHeight() / 2.0f, 220, 100);

    button_create(quit_button, 50, &mmenu_buttons,
                  GetScreenWidth() / 16.0f, play_button.bounds.y + 105, 220, 100);

    button_create(custom_level, 25, &mmenu_buttons,
                  GetScreenWidth() / 16.0f, quit_button.bounds.y + 125, 220, 85);

    // settings menu open button
    static bool settings_menu = false;

    // custom levels menu
    static bool levels_menu = false;

    gui_panel levels_pannel = {
        .pos = {GetScreenWidth() / 2.0f - 500, GetScreenHeight() / 2.0f - 300},
        .size = {1000, 600},
        .color = DARKGRAY,
        .toggle = &levels_menu,
        .exit_pos = {940, 30},
        .exit_size = {50, 50},
    };

    // needed for the texture of the button
    Rectangle settings_bounds = {
        .x = GetScreenWidth() - 100.0f,
        .y = GetScreenHeight() - 100.0f,
        .width = (float)mmenu_settings.width * 0.75f,
        .height = (float)mmenu_settings.height * 0.75f,
    };

    button_create(settings_toggle, 1, &gui_transparent, settings_bounds.x, settings_bounds.y,
                  settings_bounds.width, settings_bounds.height);

    // settings pannel
    gui_panel settings_pannel = {
        .pos = {GetScreenWidth() / 2.0f - 500, GetScreenHeight() / 2.0f - 300},
        .size = {1000, 600},
        .color = DARKGRAY,
        .toggle = &settings_menu,
        .exit_pos = {940, 30},
        .exit_size = {50, 50},
    };

    static const gui_color_scheme settings_buttons = {
        .default_color = DARKGRAY,
        .hoovered_color = GRAY,
        .pressed_color = ColorBrightness(GRAY, -0.5),
        .text_color = WHITE,
    };

    // buttons inside settings pannel

    button_create(fullscreen_setting, 20, &settings_buttons, 50, 160, 100, 50);
    button_create(camera_tilt_setting, 20, &settings_buttons, 50, 260, 100, 50);
    button_create(play_music_setting, 20, &settings_buttons, 50, 360, 100, 50);

    if (g_settings.play_music) {
        UpdateMusicStream(menu_music);
    }

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

#ifdef DEBUG
        DrawText("Debug build", GetScreenWidth() - 150, GetScreenHeight() - 250, 20, WHITE);
#endif // DEBUG

        draw_tittle();

        if (!settings_menu & !levels_menu) {

            if (IsKeyPressed(KEY_ENTER)) {
                PlaySound(g_sounds.generic_click);
                parent->swap_scene(SCENE_LEVEL_TEST1);
            }

            if (gui_button_ex(&play_button, "Play")) {
                PlaySound(g_sounds.generic_click);
                parent->swap_scene(SCENE_LEVEL_TEST1);
            }

            if (gui_button_ex(&quit_button, "Quit")) {
                PlaySound(g_sounds.generic_click);
                close_application = true;
            }

            if (gui_button_ex(&custom_level, "Custom Levels")) {
                PlaySound(g_sounds.generic_click);
                levels_menu = !levels_menu;
            }
        }

        {
            if (gui_button_ex(&settings_toggle, "")) {
                PlaySound(g_sounds.generic_click);
                settings_menu = !settings_menu;
            }
            DrawTextureEx(mmenu_settings, {settings_bounds.x, settings_bounds.y}, 0, 0.75, WHITE);
        }

        if (settings_menu) {
            float offset = 130;
            draw_panel(&settings_pannel);
            draw_text_in_pannel_space(&settings_pannel, "Settings", 50, {30, 30});

            text_settings("Fullscreen");
            if (gui_button_on_pannel(&settings_pannel, &fullscreen_setting,
                                     bool2enabledisable(g_settings.fullscreen))) {
                PlaySound(menu_click);
                g_settings.fullscreen = !g_settings.fullscreen;
                ToggleFullscreen();
            }

            text_settings("Camera Tilt");
            if (gui_button_on_pannel(&settings_pannel, &camera_tilt_setting,
                                     bool2enabledisable(g_settings.camera_tilt))) {
                PlaySound(menu_click);
                g_settings.camera_tilt = !g_settings.camera_tilt;
            }

            text_settings("Music");
            if (gui_button_on_pannel(&settings_pannel, &play_music_setting,
                                     bool2enabledisable(g_settings.play_music))) {

                PlaySound(menu_click);
                g_settings.play_music = !g_settings.play_music;
            }
        }

        if (levels_menu) {
            draw_panel(&levels_pannel);
        }
    }
    EndDrawing();
}
