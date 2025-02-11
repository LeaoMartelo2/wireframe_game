#!/bin/sh

set -xe

gcc lerp.c -L ../raylib/linux/ -lraylib -lm -o lerp

