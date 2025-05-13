#include "scene.h"
#include "collision.h"
#include "doors.h"
#include "globals.h"
#include "include/json.hpp"
#include "include/lognest.h"
#include "player.h"
#include <fstream>

#define GEOMETRY_COLOR DARKGRAY;
#define FLOOR_COLOR GetColor(0x181818FF)
#define OUTLINE_COLOR RED

Scene::Scene() {
}

Scene::~Scene() {
    lognest_trace("[Scene] Scene unloaded. ID: %zu", scene_id);
}

void Scene::start() {

    DisableCursor();

    loadmap(map_file.c_str());
    player->collider.pos = start_pos;
    player->camera.target = looking_at;
}

void Scene::end(void) {
}

void Scene::set_map(const char *filename) {

    map_file = filename;
}

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

                door.collider_a.size = {base_size.x / 2, base_size.y, base_size.z};
                door.collider_a.pos = {base_pos.x + door.collider_a.size.x / 2, base_pos.y, base_pos.z};
                door.open_pos.pos_a = {base_pos.x + door.collider_a.size.x, base_pos.y, base_pos.z};
                door.collider_a.populate();
                door.collider_a.color = GEOMETRY_COLOR;
                door.collider_a.outline_color = BLUE;

                door.collider_b.size = {base_size.x / 2, base_size.y, base_size.z};
                door.collider_b.pos = {base_pos.x - door.collider_b.size.x / 2, base_pos.y, base_pos.z};
                door.open_pos.pos_b = {base_pos.x - door.collider_b.size.x, base_pos.y, base_pos.z};
                door.collider_b.populate();
                door.collider_b.color = GEOMETRY_COLOR;
                door.collider_b.outline_color = BLUE;

                door.open_trigger.size = {base_size.x, base_size.y, 50};
                door.open_trigger.pos = {base_pos.x, base_pos.y, base_pos.z - 25};

                map_doors.push_back(door);
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

void Scene::update(void) {

    player->update(map_colliders, map_doors);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(player->camera);
        {

            player->draw();
            player->debug_3d();
            draw_scene_colliders();

            map_doors[0].update(player->collider);
            map_doors[0].draw();
        }
        EndMode3D();

        player->draw_hud();
    }
    EndDrawing();
}
