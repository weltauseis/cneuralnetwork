#! /usr/bin/env fish
gcc -Wall -o myneuralnet src/main.c src/dataset.c
./myneuralnet
