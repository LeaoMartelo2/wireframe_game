#ifndef ENEMIES_H
#define ENEMIES_H

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include <stdint.h>

enum EnemySpriteIndex{

    ENEMY_NONE = 0,
    ENEMY_GRUNT,

};

class Enemy {
  public:
    Vector3 pos;
    Vector3 hitbox_size;

    EnemySpriteIndex sprite_idx;
    size_t anim_frame;

    Texture2D sprite;

    Enemy();
    ~Enemy();

    void load();

    void update();

    void draw(Camera *camera);
};

#endif // !ENEMIES_H_
