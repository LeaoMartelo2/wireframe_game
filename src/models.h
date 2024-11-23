#ifndef MODELS_H_
#define MODELS_H_
#include <raylib.h>

typedef struct Ground {
        Vector3 size;
        Mesh mesh;
        Model model;
        Vector3 pos;
} Ground;

typedef struct Solid {
        Vector3 size;
        Mesh mesh;
        Model model;
        Vector3 pos;
} Solid;

void load_map(Solid *solids, int *count, Ground *ground_geometry);

void draw_map(const Solid *solids, int count, const Ground *ground_plates);

#endif // !MODELS_H_
