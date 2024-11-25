#include "drawloop.h"

Model menu_shotgun = {0};
Vector3 menu_shotgun_pos = {5, 0, 0};
double menu_rot = 0;

void load_menu(MainMenu *menu) {
    menu->music = LoadMusicStream("music/Menu_4.mp3");

    Rectangle play = {
        .x = GetScreenWidth() / 18.0f,
        .y = GetScreenHeight() / 1.5f - 50.0f,
        .width = 400,
        .height = 70,
    };

    menu->button = play;

    menu->text = "Play";
    menu->text_color = GRAY;

    menu->menu_camera.up = (Vector3){0, 1, 0};
    menu->menu_camera.target = (Vector3){0, 0, 0};
    menu->menu_camera.position = (Vector3){0, 0, 10};
    menu->menu_camera.fovy = 90;
    menu->menu_camera.projection = CAMERA_PERSPECTIVE;

    PlayMusicStream(menu->music);

    menu_shotgun = LoadModel(SHOTGUN_MODEL);
}

void update_menu(MainMenu *menu, Screen *current_screen) {
    UpdateMusicStream(menu->music);
    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, menu->button)) {
        menu->text_color = WHITE;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            *current_screen = SCREEN_GAME;
        }
    } else {
        menu->text_color = GRAY;
    }
}

void draw_menu(MainMenu *menu) {

    ClearBackground(BLACK);

    menu_rot++;

    BeginMode3D(menu->menu_camera);

    DrawModelEx(menu_shotgun, menu_shotgun_pos, Vector3One(), menu_rot,
                (Vector3){10, 10, 10}, GetColor(0x181818FF));
    DrawModelWiresEx(menu_shotgun, menu_shotgun_pos, Vector3One(), menu_rot,
                     (Vector3){10, 10, 10}, WHITE);

    EndMode3D();

    DrawRectangleGradientEx(menu->button,
                            DARKGRAY,
                            DARKGRAY,
                            BLACK,
                            BLACK);

    int text_width = MeasureText(menu->text, 50);
    DrawText("Play", menu->button.x + (menu->button.width / 2.0f) - text_width / 2.0f,
             menu->button.y, 50, menu->text_color);

    DrawText("Tittle goes here", 150, 50, 100, WHITE);
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
