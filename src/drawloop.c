#include "drawloop.h"

void draw_menu() {

    while (!WindowShouldClose()) {

        BeginDrawing();
        {
            ClearBackground(GetColor(0x181818FF));

            DrawText("Game Name", GetScreenHeight() / 2, GetScreenWidth() / 2 / 2, 100, WHITE);

            DrawRectangle(GetScreenWidth() / 2 - 100, GetScreenHeight() / 1.5 - 50, 200, 100, DARKGRAY);
        }
        EndDrawing();
    }
    return;
}

void main_drawloop(Player *player, Prop *props) {

    DisableCursor();
    init_player(player);
    load_map(player->geometry, &player->geometry_count, player->ground_geometry);
    load_props(props);

    while (!WindowShouldClose()) {

        move_player(player);
        update_player(player);

        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode3D(player->camera);
            {
                draw_map(player->geometry, player->geometry_count, player->ground_geometry);

                draw_props(props);

                draw_viewmodel(player);
            }
            EndMode3D();

            DrawFPS(1, 1);

            DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1, GREEN);
            DrawText(TextFormat("x:%f\ny:%f\nz:%f", player->postition.x, player->postition.y, player->postition.z),
                     5, 30, 30, WHITE);

            DrawText(TextFormat("velo forward:%f\nvelo sideways:%f\nvelo vertical:%f",
                                player->velocity.x, player->velocity.z, player->velocity.y),
                     5, 150, 30, WHITE);
        }
        EndDrawing();
    }
    return;
}
