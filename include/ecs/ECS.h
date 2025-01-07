#pragma once

#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "core/Collections.h"
#include "core/Logger.h"
#include "core/SmartPointers.h"

constexpr unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

////////////////////////////////////////////////////////////////////////////////
// Component
////////////////////////////////////////////////////////////////////////////////
struct IComponent {
protected:
    static int nextId;
};

// ReSharper disable once CppTemplateParameterNeverUsed
template<typename T>
class Component : public IComponent {
public:
    static int GetId() {
        static auto id = nextId++;
        return id;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Entity
////////////////////////////////////////////////////////////////////////////////
class Entity {
    int id;

public:
    class Registry *registry;

    explicit Entity(const int id): id{id}, registry{nullptr} {}
    Entity(const Entity &entity) = default;
    void Kill() const;
    [[nodiscard]] int GetId() const;

    void Tag(const std::string &tag) const;
    [[nodiscard]] bool HasTag(const std::string &tag) const;
    void Group(const std::string &group) const;
    [[nodiscard]] bool BelongsToGroup(const std::string &group) const;

    template<typename TComponent, typename... TArgs>
    Entity& AddComponent(TArgs &&... args);
    template<typename TComponent>
    void RemoveComponent() const;
    template<typename TComponent>
    [[nodiscard]] bool HasComponent() const;
    template<typename TComponent>
    TComponent &GetComponent() const;

    Entity &operator =(const Entity &other) = default;
    bool operator ==(const Entity &other) const { return id == other.id; }
    bool operator !=(const Entity &other) const { return id != other.id; }
    bool operator >(const Entity &other) const { return id > other.id; }
    bool operator <(const Entity &other) const { return id < other.id; }
};

////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////
class System {
    Signature componentSignature;
    List<Entity> entities;

public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(const Entity &entity);
    void RemoveEntityFromSystem(Entity entity);
    [[nodiscard]] List<Entity> GetSystemEntities() const;
    [[nodiscard]] const Signature &GetComponentSignature() const;

    template<typename TComponent>
    void RequireComponent();
};

////////////////////////////////////////////////////////////////////////////////
// Pool
////////////////////////////////////////////////////////////////////////////////
class IPool {
public:
    virtual ~IPool() = default;
    virtual void RemoveEntityFromPool(int entityId) = 0;
};

template<typename T>
class Pool final : public IPool {
    List<T> data;
    int size;

    Dictionary<int, int> entityIdToIndex;
    Dictionary<int, int> indexToEntityId;

public:
    explicit Pool(int capacity = 100) {
        size = 0;
        data.resize(capacity);
    }

    ~Pool() override = default;

    bool IsEmpty() const {
        return size == 0;
    }

    int GetSize() const {
        return size;
    }

    void Clear() {
        data.clear();
        entityIdToIndex.clear();
        indexToEntityId.clear();
        size = 0;
    }

    void Set(int entityId, T object) {
        if (entityIdToIndex.contains(entityId)) {
            auto index = entityIdToIndex[entityId];
            data[index] = object;
        } else {
            auto index = size;
            entityIdToIndex.emplace(entityId, index);
            indexToEntityId.emplace(index, entityId);
            if (index >= data.capacity()) {
                data.resize(size * 2);
            }
            data[index] = object;
            size++;
        }
    }

    void Remove(const int entityId) {
        auto indexOfRemoved = entityIdToIndex[entityId];
        auto indexOfLast = size - 1;
        data[indexOfRemoved] = data[indexOfLast];

        const auto entityIdOfLastElement = indexToEntityId[indexOfLast];
        entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
        indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

        entityIdToIndex.erase(entityId);
        indexToEntityId.erase(indexOfLast);

        size--;
    }

    void RemoveEntityFromPool(const int entityId) override {
        if (entityIdToIndex.contains(entityId)) {
            Remove(entityId);
        }
    }

    T &Get(const int entityId) {
        auto index = entityIdToIndex[entityId];
        return static_cast<T &>(data[index]);
    }

    T &operator [](unsigned int index) {
        return data[index];
    }
};

////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////
class Registry : public std::enable_shared_from_this<Registry> {
    int numEntities{0};
    List<Shared<IPool> > componentPools;
    List<Signature> entityComponentSignatures;
    Dictionary<std::type_index, Shared<System> > systems;
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;
    Dictionary<std::string, Entity> entityPerTag;
    Dictionary<int, std::string> tagPerEntity;
    Dictionary<std::string, std::set<Entity> > entitiesPerGroup;
    Dictionary<int, std::string> groupPerEntity;

    std::deque<int> freeIds{};

public:
    Registry() {
        LOG("Registry constructor called");
    }

    ~Registry() {
        LOG("Registry destructor called");
    }

    void Update();

    Entity CreateEntity();
    void KillEntity(const Entity &entity);

    void TagEntity(Entity entity, const std::string &tag);
    bool EntityHasTag(const Entity &entity, const std::string &tag) const;
    Entity GetEntityByTag(const std::string &tag) const;
    void RemoveEntityTag(const Entity &entity);

    void GroupEntity(Entity entity, const std::string &group);
    bool EntityBelongsToGroup(const Entity &entity, const std::string &group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string &group) const;
    void RemoveEntityGroup(const Entity &entity);

    template<typename TComponent, typename... TArgs>
    void AddComponent(const Entity& entity, TArgs &&... args);
    template<typename TComponent>
    void RemoveComponent(Entity entity);
    template<typename TComponent>
    bool HasComponent(Entity entity) const;
    template<typename TComponent>
    TComponent &GetComponent(Entity entity) const;

    template<typename TSystem, typename... TArgs>
    void AddSystem(TArgs &&... args);
    template<typename TSystem>
    void RemoveSystem();
    template<typename TSystem>
    bool HasSystem() const;
    template<typename TSystem>
    TSystem &GetSystem() const;

    void AddEntityToSystems(const Entity &entity) const;
    void RemoveEntityFromSystems(const Entity &entity) const;
};

template<typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template<typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&... args) {
    Shared<TSystem> newSystem = std::make_shared<TSystem>(args...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem() {
    const auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {
    return systems.contains(std::type_index(typeid(TSystem)));
}

template<typename TSystem>
TSystem &Registry::GetSystem() const {
    const auto system = systems.find(std::type_index(typeid(TSystem)));
    return *std::static_pointer_cast<TSystem>(system->second);
}

template<typename TComponent, typename... TArgs>
void Registry::AddComponent(const Entity& entity, TArgs &&... args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId]) {
        Shared<Pool<TComponent> > newComponentPool(new Pool<TComponent>());
        componentPools[componentId] = newComponentPool;
    }

    Shared<Pool<TComponent> > componentPool = std::static_pointer_cast<Pool<TComponent> >(componentPools[componentId]);

    TComponent newComponent(args...);

    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);

    LOG("Component {}, with id = {} was added to entity id {}", typeid(TComponent).name(), componentId, entityId);
}

template<typename TComponent>
void Registry::RemoveComponent(const Entity entity) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    std::shared_ptr<Pool<TComponent> > componentPool = std::static_pointer_cast<Pool<TComponent> >(
        componentPools[componentId]);
    componentPool->Remove(entityId);

    entityComponentSignatures[entityId].set(componentId, false);

    LOG("Component {} with id: {} was removed from entity with id: {}", typeid(TComponent).name(), componentId, entityId);
}

template<typename TComponent>
bool Registry::HasComponent(const Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
TComponent &Registry::GetComponent(const Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent> >(componentPools[componentId]);
    return componentPool->Get(entityId);
}

template<typename TComponent, typename... TArgs>
Entity& Entity::AddComponent(TArgs &&... args) {
    registry->AddComponent<TComponent>(*this, args...);
    return *this;
}

template<typename TComponent>
void Entity::RemoveComponent() const {
    registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent &Entity::GetComponent() const {
    return registry->GetComponent<TComponent>(*this);
}
