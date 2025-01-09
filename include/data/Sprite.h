#pragma once

#include "core/SmartPointers.h"

struct Sprite {
    int width;
    int height;
    Shared<SDL_Texture> texture;

    [[nodiscard]] SDL_Rect TextureRect() const {  return SDL_Rect{ 0, 0, width, height };  }
};
