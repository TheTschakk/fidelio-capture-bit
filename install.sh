#!/bin/bash

gcc -o test src/test.c -O3 -lm
gcc -o main src/main.c -O3 -lm -Wall

