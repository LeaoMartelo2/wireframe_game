#include "../raylib/raylib.h"
#include "../raylib/rcamera.h"
#include "drawloop.h"
#include "models.h"
#include "player.h"
#include <stdio.h>

#define MAX_SOLIDS 10

typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
} Screen;

int main(void) {

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Game");
    /*SetWindowState(FLAG_WINDOW_RESIZABLE);*/
    /*MaximizeWindow();*/
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    ToggleFullscreen();

    Screen current_screen = 1;

    switch (current_screen) {
    case SCREEN_MENU: {
        printf("turn down for what\n");
        draw_menu();
    } break;

    case SCREEN_GAME: {

        Player player = {0};
        Prop props[10];

        main_drawloop(&player, props);

    } break;
    }

    CloseWindow();

    return 0;
}
