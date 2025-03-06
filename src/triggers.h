#ifndef TRIGGERS_H_
#define TRIGGERS_H_

#include "../raylib/raylib.h"
#include "player.h"
#include <stddef.h>

class Scene;

typedef enum {
    TRIGGER_TELEPORT = 0,
    TRIGGER_LOADLEVEL,
    TRIGGER_GOTO_SCENE
} TRIGGER_TYPES;

typedef struct {
    Vector3 teleport;
    char *levelname;
    size_t scene_id;
} trigger_info;

typedef struct Trigger {
    Vector3 pos;
    Vector3 size;
    size_t id;
    int type;
    trigger_info info;
} Trigger;

void debug_draw_trigger(Trigger *trigger);
bool trigger_player_collision(Trigger *trigger, Player *player);
void update_trigger(Scene *parent, Trigger *trigger, Player *player);

#endif // !TRIGGERS_H_
