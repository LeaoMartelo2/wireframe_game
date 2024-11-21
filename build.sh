#!/bin/bash

set -xe

gcc main.c player.c -Wall -Wextra -pedantic -lraylib -lm -o main

./main

