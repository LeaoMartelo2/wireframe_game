#include "geometry.h"
#include "dynamic_arrays.h"

void geometry_draw(da_Geometry *map_geometry) {

    for (int i = 0; i < map_geometry->count; ++i) {

        DrawModel(map_geometry[i].items->model,
                  map_geometry[i].items->pos,
                  1.0f, GetColor(0x181818FF));

        DrawModelWires(map_geometry[i].items->model,
                       map_geometry[i].items->pos,
                       1.0f, RED);
    }
}
