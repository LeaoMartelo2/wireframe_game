#include "triggers.h"
#include "globals.h"
#include "include/lognest.h"
#include "player.h"
#include "scene_manager.h"

void debug_draw_trigger(Trigger *trigger) {

    DrawCubeV(trigger->pos, trigger->size, ColorAlpha(ORANGE, 0.3f));
    DrawCubeWiresV(trigger->pos, trigger->size, ORANGE);
}

bool trigger_player_collision(Trigger *trigger, Player *player) {

    Vector3 other_pos = trigger->pos;
    Vector3 other_size = trigger->size;

    Vector3 negative_other = {other_pos.x - other_size.x / 2,
                              other_pos.y - other_size.y / 2,
                              other_pos.z - other_size.z / 2};

    Vector3 positive_other = {other_pos.x + other_size.x / 2,
                              other_pos.y + other_size.y / 2,
                              other_pos.z + other_size.z / 2};

    BoundingBox floor_bounding_box = {negative_other, positive_other};

    Vector3 player_size = player->collision.bounding_box_size;

    Vector3 c_negative_other = {player->pos.x - player_size.x / 2,
                                player->pos.y - player_size.y / 2,
                                player->pos.z - player_size.z / 2};

    Vector3 c_positive_other = {player->pos.x + player_size.x / 2,
                                player->pos.y + player_size.y / 2,
                                player->pos.z + player_size.z / 2};

    BoundingBox cube_bbox = {c_negative_other, c_positive_other};

    if (CheckCollisionBoxes(cube_bbox, floor_bounding_box)) {

        return true;
    }

    return false;
}

void update_trigger(Scene *parent, Trigger *trigger, Player *player) {

    SceneManager *sm = parent->parent;

    if (trigger_player_collision(trigger, player)) {

        switch (trigger->type) {

        case TRIGGER_TELEPORT: {

            lognest_trace("[Trigger] Type: Teleport. Attempting to teleport player to: %.2f, %.2f, %.2f",
                          trigger->info.teleport.x, trigger->info.teleport.y, trigger->info.teleport.z);

            player->set_pos(trigger->info.teleport);

        } break;

        case TRIGGER_LOADLEVEL: {
            size_t new_scene = sm->add_scene_by_name("levels/level4");

            if (new_scene != sm->current_scene) {
                sm->swap_scene(new_scene);
            }
        } break;

        case TRIGGER_GOTO_SCENE: {

        } break;

        default:
            lognest_error("[Trigger] Unexpected Trigger type value: '%d'");
            lognest_error(" ┗>[Trigger] UNREACHABLE STATEAMENT AT "
                          "'%s' -> '%s:%d', Exiting.",
                          __FUNCTION__, __FILE__, __LINE__);

            close_application = true;
        }
    }
}
