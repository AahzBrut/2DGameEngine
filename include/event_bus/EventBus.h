#pragma once

#include <any>
#include <functional>
#include <list>
#include <map>
#include <typeindex>

#include "event_bus/Event.h"

class IEventCallback {
    virtual void Call(Event &e) = 0;

public:
    virtual ~IEventCallback() = default;

    virtual bool IsSameAddress(void *otherInstance) = 0;

    void Execute(Event &e) {
        Call(e);
    }
};

template<typename TOwner, typename TEvent>
class EventCallback final : public IEventCallback {
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *ownerInstance;
    CallbackFunction callbackFunction;

    void Call(Event &e) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent &>(e));
    }

    bool IsSameAddress(void *otherInstance) override {
        return static_cast<void*>(ownerInstance) == otherInstance;
    }

public:
    EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction) {
        this->ownerInstance = ownerInstance;
        this->callbackFunction = callbackFunction;
    }

    ~EventCallback() override = default;

    bool IsSameInstance(TOwner *otherInstance) { return ownerInstance == otherInstance; }
};

typedef std::list<std::unique_ptr<IEventCallback> > HandlerList;

class EventBus {
    std::map<std::type_index, std::unique_ptr<HandlerList> > subscribers;

public:
    EventBus() {
        LOG("EventBus constructor called!");
    }

    ~EventBus() {
        LOG("EventBus destructor called!");
    }

    void Reset() {
        subscribers.clear();
    }

    template<typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner *ownerInstance, void (TOwner::*callbackFunction)(TEvent &)) {
        if (!subscribers[typeid(TEvent)].get()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        subscribers[typeid(TEvent)]->push_back(
            std::make_unique<EventCallback<TOwner, TEvent> >(ownerInstance, callbackFunction));
    }

    template<typename TEvent, typename TOwner>
    void UnsubscribeFromEvent(TOwner *ownerInstance) {
        if (const auto handlers = subscribers[typeid(TEvent)].get()) {
            for (auto it = handlers->begin(); it != handlers->end(); ++it) {
                if (const auto handler = it->get(); handler->IsSameAddress(ownerInstance)) {
                    it = handlers->erase(it);
                }
            }
        }
    }

    template<typename TEvent, typename... TArgs>
    void EmitEvent(TArgs &&... args) {
        if (const auto handlers = subscribers[typeid(TEvent)].get()) {
            for (auto &it: *handlers) {
                auto handler = it.get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }
};
