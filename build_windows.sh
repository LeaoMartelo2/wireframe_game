#!/bin/bash

set -xe


x86_64-w64-mingw32-gcc src/main.c src/player.c src/models.c -std=c17 -L ./raylib/windows/ -lraylib -Wall -Wextra -pedantic -lm -lgdi32 -lwinmm -o main.exe

