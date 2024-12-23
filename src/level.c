#include "level.h"
#include "models.h"
#include <stdio.h>
#include <stdlib.h>

int load_level(const char *filepath, Solid *solids) {

    FILE *file = fopen(filepath, "r");

    if (!file) {
        fprintf(stderr, "Error: could not open file %s\n", filepath);
        return -1;
    }

    int solid_count = 0;

    if (fscanf(file, "%d", &solid_count) != 1) {
        fprintf(stderr, "Error: could not read the number of elements.\n");
        fclose(file);
        return -1;
    }

    if (solid_count < 1 || solid_count > MAX_SOLIDS) {
        fprintf(stderr, "Error: number of solids must stay between 1 and %d. Found: %d", MAX_SOLIDS, solid_count);
        fclose(file);
        return -1;
    }

    for (int i = 0; i < solid_count; i++) {
        char marker[10];

        if (fscanf(file, "%s", marker) != 1 || marker[0] != '#' || atoi(marker + 1) != i) {
            fprintf(stderr, "Error: Missing or invalid marker for solid#%d.\n", i);
            fclose(file);
            return -1;
        }

        Solid *solid = &solids[i];

        if (fscanf(file, "%f %f %f", &solid->size.x, &solid->size.y, &solid->size.z) != 3) {
            fprintf(stderr, "Error: could not read size for solid#%d.\n", i);
            fclose(file);
            return -1;
        }
    }
}
