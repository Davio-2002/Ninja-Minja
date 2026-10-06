cmake_minimum_required(VERSION 3.20)
project(Davdew_Valley)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

set(WANT_ZSTD OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    tmx
    GIT_REPOSITORY https://github.com/baylej/tmx.git
    GIT_TAG tmx_1.10.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tmx)

add_executable(Davdew_Valley
    src/main.cpp
    src/Game.cpp
    src/camera/FollowCamera.cpp
    src/input/Input.cpp
    src/map/MapRenderer.cpp
    src/map/TileAnimation.cpp
    src/map/TileMap.cpp
    src/map/TmxTextures.cpp
    src/player/Player.cpp
)
target_include_directories(Davdew_Valley PRIVATE src)
target_link_libraries(Davdew_Valley PRIVATE raylib tmx)
target_compile_definitions(Davdew_Valley PRIVATE ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")
