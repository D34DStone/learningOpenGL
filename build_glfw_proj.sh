#!/bin/bash
g++ $1.cpp -o $1.out -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -std=c++17
if [[ "$2" == "run" ]]; then ./$1.out; fi