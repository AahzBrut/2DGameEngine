#pragma once
#include <SDL_keycode.h>

#include "event_bus/Event.h"

class KeyPressedEvent : public Event {
public:
    SDL_Keycode key;
    explicit KeyPressedEvent(const SDL_Keycode key) : key{key} {}
};