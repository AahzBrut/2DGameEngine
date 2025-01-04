#pragma once
#include <memory>


template<typename T>
struct DefaultDeleter {
    void operator()([[maybe_unused]] T* pointer) const {}
};

template<typename T>
using Unique = std::unique_ptr<T, DefaultDeleter<T>>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

