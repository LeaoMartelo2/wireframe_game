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

void Scene::end() {
}

void Scene::loadmap(const char *filename) {

    lognest_trace("[Scene] Attempting to load a map from '%s'", filename);

    std::ifstream file(filename);

    if (!file.is_open()) {
        lognest_error("[Scene] Could not open file %s, exiting with status '1'", filename);
        exit(1);
        return;
    }

    nlohmann::json j;

    file >> j;

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

        lognest_debug("[Scene] Sucessfully loaded a geometry from '%s'. "
                      "%d Entries loaded so far.",
                      filename, ++i);
    }

    lognest_trace("[Scene] Sucessfully loaded '%d' Geometries from '%s' in the scene.", i, filename);
}

void Scene::draw_map_geometry() {

    for (size_t i = 0; i < map_geometry.size(); ++i) {

        geometry_draw(&map_geometry[i]);
    }
}

void Scene::update() {

    const int target_tickrate = 60;
    const std::chrono::milliseconds tick_duration(1000 / target_tickrate);

    auto start = std::chrono::high_resolution_clock::now();

    /*player.update(map_geometry);*/

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
            player.update(map_geometry);

            player.draw();
            player.debug_3d();

            draw_reference_point();
            draw_map_geometry();
        }
        EndMode3D();

        player.draw_hud();
    }
    EndDrawing();
}
