#include "scene.h"
#include "geometry.h"
#include "include/json.hpp"
#include "include/lognest.h"
#include "player.h"
#include "triggers.h"
#include <fstream>

Scene::Scene() {
}

Scene::~Scene() {
    lognest_trace("[Scene] Scene unloaded. ID: %zu", scene_id);
}

void Scene::start() {

    DisableCursor();

    loadmap(map_file.c_str());
    player->pos = start_pos;
    player->camera.target = looking_at;

    Trigger teste;

    teste.size = {10, 10, 10};
    teste.pos = {50, 50, 50};
    teste.type = TRIGGER_GOTO_SCENE;
    teste.info.levelname = (char *)"levels/level4";
    teste.info.scene_id = 2;
    teste.info.teleport = {0, 100, 0};

    map_trigger.emplace_back(teste);
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
        int geometry_count = 0;
        int ground_count = 0;
        for (const auto &item : j) {

            if (item["type"] == "geometry") {
                Geometry geometry;

                geometry.size.x = item["size"]["x"];
                geometry.size.y = item["size"]["y"];
                geometry.size.z = item["size"]["z"];

                geometry.pos.x = item["pos"]["x"];
                geometry.pos.y = item["pos"]["y"];
                geometry.pos.z = item["pos"]["z"];

                map_geometry.emplace_back(geometry);
                geometry_count++;
            }

            if (item["type"] == "floor") {
                Floor floor;

                floor.size.x = item["size"]["x"];
                floor.size.y = item["size"]["y"];
                floor.size.z = item["size"]["z"];

                floor.pos.x = item["pos"]["x"];
                floor.pos.y = item["pos"]["y"];
                floor.pos.z = item["pos"]["z"];

                map_floor.emplace_back(floor);
                ground_count++;
            }

            if (item["type"] == "spawnpoint") {
                start_pos.x = item["pos"]["x"];
                start_pos.y = item["pos"]["y"];
                start_pos.z = item["pos"]["z"];

                looking_at.x = item["looking_at"]["x"];
                looking_at.y = item["looking_at"]["y"];
                looking_at.z = item["looking_at"]["z"];
            }

            ++i;
        }

        lognest_debug(" ┗>[Scene] Sucessfully loaded '%d' entries from '%s/level.json' in the scene.",
                      i, filename);
    }
}

void Scene::draw_map_geometry(void) {

    for (size_t i = 0; i < map_geometry.size(); ++i) {

        geometry_draw(&map_geometry[i]);
    }
}

void Scene::draw_map_floor(void) {

    for (size_t i = 0; i < map_floor.size(); ++i) {
        floor_draw(&map_floor[i]);
    }
}

void Scene::update_map_triggers(void) {

    /*for(size_t = 0; i<map_trigger.size(); ++i){ */
    for (Trigger i : map_trigger) {
        update_trigger(this, &i, player);
    }
}

void Scene::debug_draw_map_triggers(void) {

    for (Trigger i : map_trigger) {

        debug_draw_trigger(&i);
    }
}

void Scene::update(void) {

#ifndef DEBUG
    player->update(map_geometry, map_floor);
#endif // !DEBUG

    update_map_triggers();

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(player->camera);
        {
#ifdef DEBUG
            player->update(map_geometry, map_floor);
#endif // DEBUG

            draw_map_floor();
            draw_map_geometry();

            player->draw();
            player->debug_3d();
            debug_draw_map_triggers();
        }
        EndMode3D();

        player->draw_hud();
    }
    EndDrawing();
}
