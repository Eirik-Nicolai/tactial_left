cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "-O0 -Wno-deprecated-literal-operator -Wno-multichar -Wno-deprecated -Wno-narrowing")

project(tactical_leftist)

set(SRC_MAIN
    src/game.cpp
    src/main.cpp
    src/logger.cpp
)

set(SRC_LAYERS
    src/layers/debug_layer.cpp
    src/layers/menu_layer.cpp
    src/layers/game_layer.cpp
)

set(SRC_STATES
    # src/states/star/starstate.cpp
    # src/states/init/initstate.cpp
    # src/states/load/loadstate.cpp
    src/states/combat/combatstate.cpp
)

set(SRC_ENGINE
    src/engine/input_handler.cpp
    src/engine/sound.cpp
)

set(SRC_SYSTEMS
    src/systems/animation.cpp
    src/systems/rendering.cpp
)

set(SRC_GAME
    ${SRC_MAIN}
    ${SRC_LAYERS}
    ${SRC_STATES}
    ${SRC_ENGINE}
    ${SRC_SYSTEMS}
)

message ("Adding executable...")
add_executable(tactical_leftist ${SRC_GAME})

message("Linking dirs....")

target_link_libraries(
    tactical_leftist
    GL
    GLU
    X11
    png
    pthread
    stdc++fs
)

target_include_directories(tactical_leftist
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
)
