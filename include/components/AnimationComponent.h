#pragma once
#include <SDL_rect.h>

#include "core/Collections.h"


struct AnimationComponent {
    List<SDL_Rect> frames;
    int currentFrame;
    Uint32 delayBetweenFrames;
    bool loop;
    Uint32 startTime;

    explicit AnimationComponent(
        const List<SDL_Rect> &frames = List<SDL_Rect>(),
        const int current_frame = 0,
        const float frames_per_second = 0.0f,
        const bool loop = false,
        const Uint32 start_time = SDL_GetTicks())
        : frames(frames),
          currentFrame(current_frame),
          delayBetweenFrames(static_cast<Uint32>(1000.f / frames_per_second)),
          loop(loop),
          startTime(start_time) {}
};
