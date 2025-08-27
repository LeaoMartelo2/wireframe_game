#ifndef ENEMIES_H
#define ENEMIES_H

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include "player.h"
#include <stdint.h>
#include <vector>

typedef struct Enemy_body_parts {

    Model model;
    Vector3 offset;
    bool face_player;

} Enemy_body_parts;

enum EnemySpriteIndex {

    ENEMY_NONE = 0,
    ENEMY_GRUNT,

};

class Enemy {
  public:
    struct {
        Vector3 pos;
        Vector3 size;
    } hitbox;

    std::vector<Enemy_body_parts *> body_models;

    Vector3 look_pos;

    Vector3 forward;

    Enemy();
    ~Enemy();

    void load();

    void update(GenericPlayerData_share player);

    void draw(Camera *camera);
};

#endif // !ENEMIES_H_
