#!/bin/bash

set -xe

gcc main.c rlFPCamera.c -Wall -Wextra -pedantic -lraylib -lm -o main

./main

