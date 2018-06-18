#!/bin/bash

gcc -o test src/test.c -O0 -g -lm -Wall
gcc -o main src/main.c -O3 -lm -Wall

