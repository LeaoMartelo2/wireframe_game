#include "triggers.h"

void debug_draw_trigger(Trigger *trigger) {

    DrawCubeV(trigger->pos, trigger->size, ColorAlpha(ORANGE, 0.3f));
    DrawCubeWiresV(trigger->pos, trigger->size, ORANGE);
}
