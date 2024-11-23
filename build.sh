#!/bin/bash

set -xe

# gcc src/main.c src/player.c src/models.c -std=c17 -Wall -Wextra -pedantic -lraylib -lm -o main

gcc src/main.c src/player.c src/models.c src/drawloop.c -std=c17 -L ./raylib/linux/ -lraylib -Wall -Wextra -pedantic -lm -o main

./main

