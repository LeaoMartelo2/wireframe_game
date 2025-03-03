#ifndef TRIGGERS_H_
#define TRIGGERS_H_

#include "../raylib/raylib.h"

typedef struct Trigger {
    Vector3 pos;
    Vector3 size;
} Trigger;

void debug_draw_trigger(Trigger *trigger);

#endif // !TRIGGERS_H_
