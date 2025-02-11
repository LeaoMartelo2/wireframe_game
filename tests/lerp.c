#include "../raylib/raylib.h"
#include "../raylib/raymath.h"

int main(void) {

    InitWindow(800, 600, "test");
    SetTargetFPS(60);

    Rectangle rect = {
        .x = 100,
        .y = 50,
        .width = 50,
        .height = 50,
    };

    while (!WindowShouldClose()) {

        Lerp(rect.x, rect.x + 200, 50.0f);

        BeginDrawing();
        {
            rect.x = Lerp(rect.x, rect.x + 200, 5.0f * GetFrameTime());
            DrawRectangleRec(rect, GREEN);
            ClearBackground(BLACK);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
