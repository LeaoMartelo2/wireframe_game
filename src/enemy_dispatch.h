#ifndef ENEMY_DRAW_H_
#define ENEMY_DRAW_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include "player.h"
#include "enemies.h"


void draw_cabela(const Enemy_body_parts *cabela, const Vector3 pos, const float angle);
void draw_shotgun(const Enemy_body_parts *shotgun, const Vector3 pos, const float angle);

void update_pos_right(Enemy_body_parts *part, Enemy *enemy);


#endif // !ENEMY_DRAW_H_
