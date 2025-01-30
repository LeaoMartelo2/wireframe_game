#!/bin/bash

touch latest.log
# create file if running the script before the game

tail -f latest.log | sed \
    -e 's/\(.*LOG.*\)/\x1B[32m\1\x1B[39m/' \
    -e 's/\(.*ERROR.*\)/\x1B[31m\1\x1B[39m/' \
    -e 's/\(.*DEBUG.*\)/\x1B[34m\1\x1B[39m/' \
    -e 's/\(.*WARN.*\)/\x1B[33m\1\x1B[39m/' 
