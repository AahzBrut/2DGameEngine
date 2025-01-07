cmake_minimum_required(VERSION 3.30)
project(2DGameEngine)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL2IMAGE_INSTALL OFF)
set(SDL_STATIC ON)

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

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.4
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL2 glm SDL2_image)

add_executable(2DGameEngine src/main.cpp
        include/core/Game.h
        src/core/Game.cpp
        include/core/SmartPointers.h
        include/core/Logger.h
        include/components/TransformComponent.h
        include/systems/MovementSystem.h
        include/ecs/ECS.h
        include/core/Collections.h
        src/ecs/ECS.cpp
        include/components/VelocityComponent.h
        include/systems/RenderSystem.h
        include/components/SpriteComponent.h
        include/asset_manager/AssetManager.h
        src/asset_manager/AssetManager.cpp
        include/data/Sprite.h
        include/components/AnimationComponent.h
        include/systems/AnimationSystem.h
        include/components/BoxColliderComponent.h
        include/systems/CollisionSystem.h
        include/systems/RenderColliderSystem.h
)

target_link_libraries(2DGameEngine PRIVATE SDL2::SDL2main SDL2::SDL2-static glm SDL2_image::SDL2_image-static)
target_include_directories(2DGameEngine PUBLIC include)


add_custom_command(TARGET 2DGameEngine POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_LIST_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets")
