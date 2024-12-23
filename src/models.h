#ifndef MODELS_H_
#define MODELS_H_

#include "../raylib/raylib.h"

#define MAX_SOLIDS 100

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
        bool see_through;
} Solid;

typedef struct Prop {
        Vector3 size;
        Mesh mesh;
        Model model;
        Vector3 pos;
        bool bounce; // model moves up and down
        bool rotate;
} Prop;

/*void load_map(Solid *solids, int *count, Ground *ground_geometry);*/
void draw_map(const Solid *solids, int count, const Ground *ground_plates);

void load_props(Prop *props);
void draw_props(Prop *props);

#endif // !MODELS_H_
