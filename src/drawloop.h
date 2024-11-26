#ifndef DRAWLOOP_H_
#define DRAWLOOP_H_

#include "../raylib/raylib.h"
#include "models.h"
#include "player.h"

#define SHOTGUN_MODEL "models/low_poly_shotgun/shotgun.gltf"

typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
} Screen;

typedef struct MainMenu {
        Rectangle button;
        Music music;
        char *text;
        Color text_color;
        Camera3D menu_camera;

} MainMenu;

void load_menu(MainMenu *menu);
void update_menu(MainMenu *menu, Screen *current_screen);
void draw_menu(MainMenu *menu);

void load_main(Player *player, Prop *props);
void update_main(Player *player);
void main_drawloop(Player *player, Prop *props);

#endif // !DRAWLOOP_H_
