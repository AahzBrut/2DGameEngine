cmake_minimum_required(VERSION 3.30)
project(2DGameEngine)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)

include(FetchContent)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.11
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2 glm)

add_executable(2DGameEngine src/main.cpp
        include/core/Game.h
        src/core/Game.cpp
        include/core/Core.h
)

target_link_libraries(2DGameEngine PRIVATE SDL2::SDL2main SDL2::SDL2-static glm)
target_include_directories(2DGameEngine PUBLIC include)
