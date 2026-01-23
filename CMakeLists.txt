cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "-O0")

set(CXX_STANDARD "C++20")

# add_subdirectory(src)
project(tactical_leftist)

set(SRC_MAIN
    src/main.cpp
    src/game.cpp
    src/states/starstate.cpp
    src/states/initstate.cpp
    src/states/loadstate.cpp

    src/systems/rendering.cpp
)

# set(SRC_COMPONENTS
#     src/components/statemachine.cpp
# )

# set(SRC_ENGINE
#     src/engine/game.cpp
#     src/engine/menu.cpp
#     src/engine/keypress.cpp
#     src/engine/initialize.cpp
#     src/engine/asset_manager.cpp
#     src/engine/rendering/render.cpp
# )

# set(SRC_UTILS
#     src/utils/debug.cpp
# )

# set(SRC_SYSTEMS
#     src/systems/logic/logic.cpp
#     src/systems/movement/moving.cpp
#     src/systems/status/system_status.cpp
#     src/systems/animation/system_animation.cpp
#     src/systems/statemachine/system_statechange.cpp
# )

set(SRC_GAME
    ${SRC_MAIN}
    # ${SRC_ENGINE} ${SRC_COMPONENTS} ${SRC_UTILS} ${SRC_SYSTEMS}
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
