#pragma once
#include <SDL_pixels.h>
#include <string>
#include <utility>
#include <glm/vec2.hpp>

struct LabelComponent {
    using Vec = glm::vec2;
    using String = std::string;

    Vec position;
    String text;
    Shared<TTF_Font> font;
    SDL_Color color;
    bool fixed;

    explicit LabelComponent(
        const Vec &position = Vec{0},
        String text = "",
        const Shared<TTF_Font>& font = Shared<TTF_Font>{nullptr},
        const SDL_Color &color = SDL_Color{255, 255, 255, 255},
        const bool fixed = true)
        : position{position},
          text{std::move(text)},
          font{font},
          color{color},
          fixed{fixed} {}
};
