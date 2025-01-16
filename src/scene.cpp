#include "scene.h"
#include "geometry.h"
#include "include/json.hpp"
#include "include/lognest.h"
#include "player.h"
#include <chrono>
#include <fstream>
#include <thread>

Scene::Scene() : player() {
    lognest_trace("[Scene] Scene constructed.");
}

Scene::~Scene() {
    lognest_trace("[Scene] Scene destructed.");
}

void Scene::start(const char *filename) {

    loadmap(filename);
}

void Scene::end(void) {
}

void Scene::loadmap(const char *filename) {

    lognest_trace("[Scene] Attempting to load Level from from '%s'.", filename);

    std::ifstream file_geometry(std::string(filename) + "/geometry.json");
    lognest_trace("[Scene] Trying to load Geometry(ies) from '%s/geometry.json'", filename);

    if (!file_geometry.is_open()) {
        lognest_error("[Scene] Could not open file '%s/geometry.json' whilist attemping to load Geometries.",
                      filename);
    } else {

        nlohmann::json j;

        file_geometry >> j;

        int i = 0;
        for (const auto &item : j) {
            Geometry geometry;

            geometry.size.x = item["size"]["x"];
            geometry.size.y = item["size"]["y"];
            geometry.size.z = item["size"]["z"];

            geometry.pos.x = item["pos"]["x"];
            geometry.pos.x = item["pos"]["y"];
            geometry.pos.x = item["pos"]["z"];

            geometry.mesh = GenMeshCube(geometry.size.x,
                                        geometry.size.y,
                                        geometry.size.z);

            geometry.model = LoadModelFromMesh(geometry.mesh);

            map_geometry.emplace_back(geometry);

            lognest_debug("[Scene] Loaded a geometry from '%s/geometry.json'. "
                          "%d Entry(ies) loaded so far.",
                          filename, ++i);
        }

        lognest_trace("[Scene] Sucessfully loaded '%d' Geometries from '%s/geometry.json' in the scene.",
                      i, filename);
    }

    std::ifstream file_floor(std::string(filename) + "/floor.json");
    lognest_trace("[Scene] Trying to load Floor Tiles from '%s/floor.json'", filename);

    if (!file_floor.is_open()) {
        lognest_error("[Scene] Could not open file '%s/floor.json' whilist attemping to load Floor.",
                      filename);
    } else {

        nlohmann::json floor;

        file_floor >> floor;

        int i = 0;
        for (const auto &item : floor) {

            Floor floor;

            floor.size.x = item["size"]["x"];
            floor.size.y = item["size"]["y"];
            floor.size.z = item["size"]["z"];

            floor.pos.x = item["pos"]["x"];
            floor.pos.y = item["pos"]["y"];
            floor.pos.z = item["pos"]["z"];

            floor.mesh = GenMeshCube(floor.size.x,
                                     floor.size.y,
                                     floor.size.z);

            floor.model = LoadModelFromMesh(floor.mesh);

            map_floor.emplace_back(floor);

            lognest_debug("[Scene] Loaded a Floor tile from '%s/floor.json' in the scene. "
                          "%d Entry(ies) loaded so far.",
                          filename, ++i);
        }

        lognest_trace("[Scene] Sucessfully loaded '%d' Floor tiles from '%s/floor.json' in the scene.", i, filename);
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

void Scene::update(void) {

    const int target_tickrate = 60;
    const std::chrono::milliseconds tick_duration(1000 / target_tickrate);

    auto start = std::chrono::high_resolution_clock::now();

    player.update(map_geometry, map_floor);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    if (elapsed < tick_duration) {
        std::this_thread::sleep_for(tick_duration - elapsed);
    }

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(player.camera);
        {
            /*player.update(map_geometry);*/

            draw_reference_point();
            draw_map_floor();
            draw_map_geometry();

            player.draw();
            player.debug_3d();
        }
        EndMode3D();

        player.draw_hud();
    }
    EndDrawing();
}
