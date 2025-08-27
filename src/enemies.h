#ifndef ENEMIES_H
#define ENEMIES_H

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include "globals.h"
#include <stdint.h>
#include <vector>


typedef struct Enemy_body_parts{

    Model model;
    Vector3 offset;

}Enemy_body_parts;

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


    std::vector<Enemy_body_parts> body_models;

    Enemy();
    ~Enemy();

    void load();

    void update();

    void draw(Camera *camera);
};

#endif // !ENEMIES_H_
