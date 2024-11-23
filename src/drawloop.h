#ifndef DRAWLOOP_H_
#define DRAWLOOP_H_

#include "../raylib/raylib.h"
#include "models.h"
#include "player.h"

void draw_menu();

void main_drawloop(Player *player, Prop *props);

#endif // !DRAWLOOP_H_
