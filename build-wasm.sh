#!/bin/bash
set -euo pipefail

# Get EMSDK on the PATH
#cd /home/andy/src/clones/emsdk
#source emsdk_env.sh

# Get into the /src folder
cd /home/andy/src/github/dungeon-adventure
#cd src

# Build to Web Assembly
emcc main.c -o out/index.html -Wall \
    -I/home/andy/src/clones/raylib/src \
    -L/home/andy/src/clones/raylib/src \
    -lraylib \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    --shell-file shell.html \
    -s TOTAL_STACK=64MB \
    -s INITIAL_MEMORY=128MB \
    -s ASSERTIONS \
    -s WASM=1
    #-DPLATFORM_WEB

# Get into /out
cd out

# Run the game
#emrun index.html
    #-Os -Wall \ 
    #/Users/anguscheng/workspace/raylib/src/libraylib.a \
    #-I. -I /usr/include \
    #-l /home/andy/src/clones/raylib/src \
    #-L libarylib.a \

    #--preload-file resources \
