#pragma once
#include <chrono>
#include <iostream>
#include <memory>
#include <SDL.h>

#include "Logger.h"


template<typename T>
struct Deleter {
    void operator()(const T *pointer) const {
        LOG("Destructor for {} called", typeid(pointer).name());
        delete pointer;
    }
};

template<>
struct Deleter<SDL_Window> {
    void operator()([[maybe_unused]] const SDL_Window *pointer) const {}
};

template<>
struct Deleter<SDL_Renderer> {
    void operator()([[maybe_unused]] const SDL_Renderer *pointer) const {}
};

template<typename T>
using Unique = std::unique_ptr<T, Deleter<T> >;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;
