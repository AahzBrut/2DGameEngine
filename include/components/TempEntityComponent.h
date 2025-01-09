#pragma once


struct TempEntityComponent {
    int lifeSpanLeft;
    int birthTime{static_cast<int>(SDL_GetTicks())};

    explicit TempEntityComponent(const int lifeSpanLeft = 0)
        : lifeSpanLeft(lifeSpanLeft) {}
};
