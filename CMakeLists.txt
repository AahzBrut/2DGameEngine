cmake_minimum_required(VERSION 3.30)
project(2DGameEngine)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL2IMAGE_INSTALL OFF)
set(SDL_STATIC ON)
set(SDL2MIXER_VENDORED ON)
set(SDL2TTF_VENDORED ON)

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

FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.24.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG v3.3.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        lua
        GIT_REPOSITORY https://github.com/walterschell/Lua.git
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL2 glm SDL2_image SDL2_mixer SDL_ttf sol2 lua)

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
        include/events/CollisionEvent.h
        include/event_bus/EventBus.h
        include/event_bus/Event.h
        include/events/KeyPressedEvent.h
        include/systems/KeyboardControlSystem.h
        include/components/KeyboardControlComponent.h
        include/components/CameraFollowComponent.h
        include/systems/CameraMovementSystem.h
        include/components/HealthComponent.h
        include/components/ProjectileEmitterComponent.h
        include/data/CollisionLayer.h
        include/systems/ProjectileEmissionSystem.h
        include/components/TempEntityComponent.h
        include/systems/TempEntitiesRemovalSystem.h
        include/components/DirectionComponent.h
        include/systems/DamageSystem.h
        include/components/DamageComponent.h
        include/audio_manager/AudioManager.h
        src/audio_manager/AudioManager.cpp
        include/systems/SoundPlaySystem.h
        include/utils/Math.h
        include/events/SoundEmitterEvent.h
        include/components/LabelComponent.h
        include/systems/RenderLabelSystem.h
        include/systems/HealthBarSystem.h
        include/core/LevelLoader.h
        src/core/LevelLoader.cpp
        src/data/CollisionLayer.cpp
        include/components/ScriptComponent.h
        include/systems/ExecuteScriptSystem.h
)

target_link_libraries(2DGameEngine PRIVATE SDL2::SDL2main SDL2::SDL2-static glm SDL2_image::SDL2_image-static SDL2_mixer::SDL2_mixer-static SDL2_ttf::SDL2_ttf-static sol2::sol2 lua_static)
target_include_directories(2DGameEngine PUBLIC include)

add_custom_target(COPY_ASSETS
        COMMAND
        ${CMAKE_COMMAND} -E echo Copying assets to build directory

        COMMAND
        ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_LIST_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets"
)
add_dependencies(${CMAKE_PROJECT_NAME} COPY_ASSETS)
