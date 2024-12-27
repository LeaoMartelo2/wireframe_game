#ifndef DARRAYS_H_
#define DARRAYS_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "geometry.h"
#include <stdlib.h>

// https://gist.github.com/rexim/b5b0c38f53157037923e7cdd77ce685d
// rexim/da_append.c - Dynamic arrays in C

#define da_append(xs, x)                                                               \
    do {                                                                               \
        if ((xs)->count >= (xs)->capacity) {                                           \
            if ((xs)->capacity == 0)                                                   \
                (xs)->capacity = 256;                                                  \
            else                                                                       \
                (xs)->capacity *= 2;                                                   \
            (xs)->items = realloc((xs)->items, (xs)->capacity * sizeof(*(xs)->items)); \
        }                                                                              \
                                                                                       \
        (xs)->items[(xs)->count++] = (x);                                              \
    } while (0)

typedef struct {
        int *items;
        size_t count;
        size_t capacity;
} Numbers;

typedef struct {
        Geometry *items;
        size_t count;
        size_t capacity;
} da_Geometry;

typedef struct {
        Prop *items;
        size_t count;
        size_t capacity;
} da_Props;

#endif // !DARRAYS_H_
