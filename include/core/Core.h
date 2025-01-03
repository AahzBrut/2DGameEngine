#pragma once
#include <memory>
#include <SDL_video.h>


template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename D>
using UniqueWithDeleter = std::unique_ptr<T, D>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

