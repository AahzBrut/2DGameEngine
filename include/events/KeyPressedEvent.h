#pragma once
#include <SDL_keycode.h>

#include "event_bus/Event.h"


class KeyPressedEvent : public Event {
public:
    Uint32 eventType;
    SDL_Keycode key;
    explicit KeyPressedEvent(const Uint32 eventType, const SDL_Keycode key) : eventType{eventType}, key{key} {}
};