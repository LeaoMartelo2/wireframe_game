#!/bin/bash

set -xe

gcc src/main.c src/player.c src/models.c -Wall -Wextra -pedantic -lraylib -lm -o main

./main

