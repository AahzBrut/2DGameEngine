#pragma once

#include "core/SmartPointers.h"

struct Sprite {
    int width;
    int height;
    Shared<SDL_Texture> texture;
};
