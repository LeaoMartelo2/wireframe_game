#include "globals.h"
#include "scene.h"
#include "scene_manager.h"

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

// sligtly modified implementation of RayGui buttons
// https://github.com/raysan5/raygui
int menu_button(Rectangle bounds, const char *text, int font_size) {

    int result = 0;

    static const Color default_color = GetColor(0x181818FF);
    static const Color hoveringcolor = DARKGRAY;
    static const Color pressed_color = GRAY;

    Color current_color = default_color;

    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, bounds)) {
        current_color = hoveringcolor;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            current_color = pressed_color;
        } else {
            current_color = hoveringcolor;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result = 1;
        }

    } else {
        current_color = default_color;
    }

    DrawRectangleRec(bounds, current_color);
    if (text != NULL) {
        int text_width = MeasureText(text, font_size);

        float text_x = bounds.x + (bounds.width / 2.0f) - (text_width / 2.0f);

        float text_y = bounds.y + bounds.height / 2.0f - font_size / 2.0f;

        DrawText(text, text_x, text_y, font_size, WHITE);
    }

    return result;
}

MainMenu::MainMenu() {

    mm_camera = {0};
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

    static Sound menu_click = LoadSound(MENU_CLICK);

    static Rectangle play_button = {
        .x = GetScreenWidth() / 16.0f,
        .y = GetScreenHeight() / 2.0f,
        .width = 220,
        .height = 100,
    };

    static Rectangle quit_button = {
        .x = GetScreenWidth() / 16.0f,
        .y = play_button.y + 105,
        .width = 220,
        .height = 100,
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

        if (menu_button(play_button, "Play", 50)) {
            PlaySound(menu_click);
            parent->swap_scene(SCENE_LEVEL_SAMPLE);
        }

        if (menu_button(quit_button, "Quit", 50)) {
            close_application = true;
        }
    }
    EndDrawing();
}
