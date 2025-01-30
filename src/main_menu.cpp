#include "scene.h"

MainMenu::MainMenu() {
}

MainMenu::~MainMenu() {
}

void MainMenu::start() {
    printf("redefined start\n");
}

void MainMenu::update() {

    BeginDrawing();
    {
        ClearBackground(BLACK);

        DrawRectangle(GetScreenWidth() / 2, GetScreenHeight() / 2, 50, 50, RED);
    }
    EndDrawing();
}
