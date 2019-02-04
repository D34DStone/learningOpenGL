#!/bin/bash
g++ $1.cpp -o $1.out -lglut -lGLU -lGL -std=c++17
if [[ "$2" == "run" ]]; then ./$1.out; fi