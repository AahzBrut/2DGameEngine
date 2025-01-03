#pragma once
#include <memory>
#include <SDL_render.h>
#include <SDL_video.h>

struct SDLWindowDeleter {
    void operator()(SDL_Window* window) const {
        SDL_DestroyWindow(window);
    }
};

struct SDLRendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        SDL_DestroyRenderer(renderer);
    }
};

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename D>
using UniqueWithDeleter = std::unique_ptr<T, D>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

