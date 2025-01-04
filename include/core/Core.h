#pragma once
#include <chrono>
#include <iostream>
#include <memory>
#include <SDL.h>


template<typename T>
struct DefaultDeleter {
    void operator()([[maybe_unused]] const T *pointer) const {
        std::cerr << "Destructor called" << std::endl;
        delete pointer;
    }
};

template<>
struct DefaultDeleter<SDL_Window> {
    void operator()([[maybe_unused]] const SDL_Window *pointer) const {}
};

template<>
struct DefaultDeleter<SDL_Renderer> {
    void operator()([[maybe_unused]] const SDL_Renderer *pointer) const {}
};

template<typename T>
using Unique = std::unique_ptr<T, DefaultDeleter<T> >;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;
