#include "../raylib/raylib.h"
#include "../raylib/rcamera.h"
#include "drawloop.h"
#include "models.h"
#include "player.h"

int main(void) {

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Game");
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    ToggleFullscreen();

    InitAudioDevice();

    MainMenu menu = {0};
    load_menu(&menu);

    Player player = {0};
    Prop props[10];
    load_main(&player, props);

    Screen current_screen = 0;

    while (!WindowShouldClose()) {

        switch (current_screen) {

        case SCREEN_MENU: {

            update_menu(&menu, &current_screen);

            BeginDrawing();
            draw_menu(&menu);
            EndDrawing();

        } break;

        case SCREEN_GAME: {

            update_main(&player);

            BeginDrawing();
            main_drawloop(&player, props);
            EndDrawing();

        } break;
        }
    }

    CloseWindow();

    return 0;
}
