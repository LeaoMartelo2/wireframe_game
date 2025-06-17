#include "scene.h"
#include "collision.h"
#include "doors.h"
#include "globals.h"
#include "gui.h"
#include "include/json.hpp"
#include "include/lognest.h"
#include "items.h"
#include "player.h"
#include <fstream>

#define GEOMETRY_COLOR DARKGRAY;
#define FLOOR_COLOR GetColor(0x181818FF)
#define OUTLINE_COLOR RED

Scene::Scene() {
    map_colliders.reserve(1000);
    map_doors.reserve(10);
    map_items.reserve(100);
}

Scene::~Scene() {
    lognest_trace("[Scene] Scene unloaded. ID: %zu", scene_id);
}

void Scene::start() {

    g_gamestate.is_paused = false;

    loadmap(map_file.c_str());
    player->collider.pos = start_pos;
    player->camera.target = looking_at;
}

void Scene::end(void) {}

void Scene::set_map(const char *filename) { map_file = filename; }

void Scene::loadmap(const char *filename) {

    lognest_trace("[Scene] ID: %zu Attempting to load Level from from '%s'.", scene_id, filename);

    std::ifstream file_level(std::string(filename) + "/level.json");

    if (!file_level.is_open()) {
        lognest_error("[Scene] Could not open file '%s/level.json' whilist attemping to load level.",
                      filename);
    } else {

        nlohmann::json j;

        file_level >> j;

        int i = 0;
        for (const auto &item : j) {

            if (item["type"] == "geometry") {

                Collider geometry;

                geometry.size.x = item["size"]["x"];
                geometry.size.y = item["size"]["y"];
                geometry.size.z = item["size"]["z"];

                geometry.pos.x = item["pos"]["x"];
                geometry.pos.y = item["pos"]["y"];
                geometry.pos.z = item["pos"]["z"];

                geometry.populate();
                geometry.color = GEOMETRY_COLOR;
                geometry.outline_color = OUTLINE_COLOR;

                map_colliders.push_back(geometry);
            }

            if (item["type"] == "floor") {

                Collider floor;

                floor.size.x = item["size"]["x"];
                floor.size.y = item["size"]["y"];
                floor.size.z = item["size"]["z"];

                floor.pos.x = item["pos"]["x"];
                floor.pos.y = item["pos"]["y"];
                floor.pos.z = item["pos"]["z"];

                floor.populate();
                floor.color = FLOOR_COLOR;
                floor.outline_color = OUTLINE_COLOR;

                map_colliders.push_back(floor);
            }

            if (item["type"] == "spawnpoint") {
                start_pos.x = item["pos"]["x"];
                start_pos.y = item["pos"]["y"];
                start_pos.z = item["pos"]["z"];

                looking_at.x = item["looking_at"]["x"];
                looking_at.y = item["looking_at"]["y"];
                looking_at.z = item["looking_at"]["z"];
            }

            if (item["type"] == "door") {

                Door door;
                door.open = false;

                Vector3 base_size;
                Vector3 base_pos;

                base_size.x = item["size"]["x"];
                base_size.y = item["size"]["y"];
                base_size.z = item["size"]["z"];

                base_pos.x = item["pos"]["x"];
                base_pos.y = item["pos"]["y"];
                base_pos.z = item["pos"]["z"];

                std::string keytype = item["key_type"];

                door.set_key_type(keytype);

                door.collider_a.size = {base_size.x / 2, base_size.y, base_size.z};
                door.collider_a.pos = {base_pos.x + door.collider_a.size.x / 2, base_pos.y, base_pos.z};
                door.open_pos.pos_a = {base_pos.x + door.collider_a.size.x, base_pos.y, base_pos.z};
                door.collider_a.populate();
                door.collider_a.color = GEOMETRY_COLOR;
                door.collider_a.outline_color = door_get_color_from_keytype(keytype);

                door.collider_b.size = {base_size.x / 2, base_size.y, base_size.z};
                door.collider_b.pos = {base_pos.x - door.collider_b.size.x / 2, base_pos.y, base_pos.z};
                door.open_pos.pos_b = {base_pos.x - door.collider_b.size.x, base_pos.y, base_pos.z};
                door.collider_b.populate();
                door.collider_b.color = GEOMETRY_COLOR;
                door.collider_b.outline_color = door_get_color_from_keytype(keytype);

                door.open_trigger.size = {base_size.x, base_size.y, 50};
                door.open_trigger.pos = {base_pos.x, base_pos.y, base_pos.z - 25};

                map_doors.push_back(door);
            }

            if (item["type"] == "item") {

                DroppedItem dropped_item;

                dropped_item.pos.x = item["pos"]["x"];
                dropped_item.pos.y = item["pos"]["y"];
                dropped_item.pos.z = item["pos"]["z"];

                if (item["item_type"] == "ITEM_SHOTGUN") {
                    dropped_item.type = ITEM_SHOTGUN;
                }

                if (item["item_type"] == "ITEM_AXE") {
                    dropped_item.type = ITEM_AXE;
                }

                if (item["item_type"] == "ITEM_CABELA") {
                    dropped_item.type = ITEM_CABELA;
                }

                dropped_item.load();

                map_items.push_back(dropped_item);
            }

            if (item["type"] == "key") {

                DroppedKey key;

                key.pos.x = item["pos"]["x"];
                key.pos.y = item["pos"]["y"];
                key.pos.z = item["pos"]["z"];

                key.type = get_keytpe_from_string(item["key_type"]);

                key.load();

                map_keys.push_back(key);
            }

            if (item["type"] == "trigger") {
                continue;

                //     trigger->pos.x = item["pos"]["x"];
                //     trigger->pos.y = item["pos"]["y"];
                //     trigger->pos.z = item["pos"]["z"];
                //
                //     trigger->size.x = item["size"]["x"];
                //     trigger->size.y = item["size"]["y"];
                //     trigger->size.z = item["size"]["z"];
                //
                //     if (item["trigger_type"] == "teleport") {
                //         trigger->type = TRIGGER_TELEPORT;
                //
                //         trigger->info.teleport.x = item["info"]["teleport"]["x"];
                //         trigger->info.teleport.y = item["info"]["teleport"]["y"];
                //         trigger->info.teleport.z = item["info"]["teleport"]["z"];
                //     }
                //
                //     if (item["trigger_type"] == "goto_scene") {
                //         trigger->type = TRIGGER_GOTO_SCENE;
                //
                //         trigger->info.scene_id = item["info"]["scene_id"];
                //     }
                //
                //     if (item["trigger_type"] == "load_level") {
                //         trigger->type = TRIGGER_LOADLEVEL;
                //
                //         trigger->info.levelname = item["info"]["level_name"];
                //     }
                //
                //     trigger->populate();
                //
                //     setup_collider_mesh(trigger, trigger->model.meshes[0]);
                //
                //     map_colliders.push_back(trigger);
            }

            ++i;
        }

        lognest_debug(" ┗>[Scene] Sucessfully loaded '%d' entries from '%s/level.json' in the scene.",
                      i, filename);
    }
}

void Scene::draw_scene_colliders() {

    for (const auto &collider : map_colliders) {

        /* don't draw the geometry if its too far away */
        float distance = Vector3Distance(player->collider.pos, collider.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        collider.draw();
    }
}

void Scene::draw_scene_doors() {

    for (const auto &door : map_doors) {
        float distance = Vector3Distance(player->collider.pos, door.open_trigger.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        door.draw();
    }
}

void Scene::update_scene_doors() {

    for (auto &door : map_doors) {

        float distance = Vector3Distance(player->collider.pos, door.open_trigger.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        door.update(player->collider, &player->current_key);
    }
}

void Scene::update_scene_items() {

    for (size_t i = 0; i < map_items.size(); ++i) {
        auto &item = map_items[i];

        volatile float distance = Vector3Distance(player->collider.pos, item.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        if (item.update(player->collider.pos, player->collider.size)) {

            lognest_debug(" ┗>[Scene] Collected DroppedItem with ID: %zu , of type: '%s'",
                          i,
                          get_item_as_cstr(item.type));

            player->give_item(item.player_slot, item.type);

            map_items.erase(map_items.begin() + i);
        }
    }
}

void Scene::draw_scene_items() {

    for (auto &item : map_items) {

        float distance = Vector3Distance(player->collider.pos, item.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        item.draw();
    }
}

void Scene::update_scene_keys() {

    for (size_t i = 0; i < map_keys.size(); ++i) {

        auto &key = map_keys[i];

        float distance = Vector3Distance(player->collider.pos, key.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        if (key.update(player->collider.pos, player->collider.size, &player->current_key)) {

            lognest_debug("[Player] Gave the player the '%s' key.", get_key_as_cstr(key.type));

            map_keys.erase(map_keys.begin() + i);
        }
    }
}

void Scene::draw_scene_keys() {

    for (auto &key : map_keys) {

        float distance = Vector3Distance(player->collider.pos, key.pos);
        if (distance > g_settings.draw_distance) {
            continue;
        }

        key.draw();
    }
}

void Scene::paused_update(void) {

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(player->camera);
        {

            player->draw();
            draw_scene_colliders();
            draw_scene_doors();
            draw_scene_items();
        }
        EndMode3D();

        player->draw_hud();

        draw_pause_menu();
    }
    EndDrawing();
}

void Scene::draw_pause_menu() {

    static bool toggle = false;

    if (toggle) {
        g_gamestate.is_paused = false;
        toggle = false;
    }

    static gui_panel pause_pannel = {
        .pos = {GetScreenWidth() / 2.0f - 250, GetScreenHeight() / 2.0f - 150},
        .size = {500, 300},
        .color = DARKGRAY,
        .toggle = &toggle,
        .exit_pos = {470, 15},
        .exit_size = {25, 25},
    };

    static const gui_color_scheme pause_buttons = {
        .default_color = DARKGRAY,
        .hoovered_color = GRAY,
        .pressed_color = ColorBrightness(GRAY, -0.5),
        .text_color = WHITE,
    };

    button_create(pause_quit, 40, &pause_buttons, 50, 160, 200, 100);

    draw_panel(&pause_pannel);
    draw_text_in_pannel_space(&pause_pannel, "Paused", 50, {30, 30});
    draw_text_in_pannel_space(&pause_pannel, "Not much here yet...", 20, {30, 80});

    if (gui_button_on_pannel(&pause_pannel, &pause_quit, "Quit")) {
        PlaySound(g_sounds.generic_click);
        while (IsSoundPlaying(g_sounds.generic_click)) {
            // lol, lmao even
        }
        close_application = true;
    }
}

void Scene::update(void) {

    if (!g_gamestate.is_paused) {

        player->update(map_colliders, map_doors);

        update_scene_doors();
        update_scene_items();
        update_scene_keys();

        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode3D(player->camera);
            {

                player->draw();
                player->debug_3d();
                draw_scene_colliders();
                draw_scene_doors();
                draw_scene_items();
                draw_scene_keys();
            }
            EndMode3D();

            player->draw_hud();
        }
        EndDrawing();

    } else { // if the game is paused

        paused_update();
    }
}
