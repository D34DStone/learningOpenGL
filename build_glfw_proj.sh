#!/bin/bash

g++ -c $1.cpp -o ./obj/$1.o
g++ ./obj/$1.o ./obj/glad.o -lGL -lGLU -lglfw3 -lXxf86vm -lXrandr -pthread -lXi -ldl -lX11 -lXinerama -lXcursor -o $1.out
if [[ "$2" == "run" ]]; then ./$1.out; fi