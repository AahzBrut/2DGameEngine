#pragma once
#include <bitset>
#include <shobjidl.h>
#include <typeindex>

#include "core/Collections.h"
#include "core/SmartPointers.h"


constexpr unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

///////////////////////////////////////////////////////////////////////////////////////
/// Base Component class
///////////////////////////////////////////////////////////////////////////////////////
struct BaseComponent {
protected:
    static int nextId;
};

// ReSharper disable once CppTemplateParameterNeverUsed
template<typename T>
class Component : public BaseComponent {
public:
    static int GetId() {
        static auto id = nextId++;
        return id;
    }
};

///////////////////////////////////////////////////////////////////////////////////////
/// Entity
///////////////////////////////////////////////////////////////////////////////////////
class Entity {
    int id;

public:
    explicit Entity(const int id) : id{id} {}
    [[nodiscard]] int GetId() const { return id; }

    bool operator==(const Entity &other) const { return id == other.id; }
};

template<>
struct std::hash<Entity> {
    size_t operator()(const Entity &id) const noexcept {
        return hash<int>()(id.GetId());
    }
};

///////////////////////////////////////////////////////////////////////////////////////
/// Base System class
///////////////////////////////////////////////////////////////////////////////////////
class System {
    Signature componentsSignature;
    List<Entity> entities;

public:
    void AddEntity(Entity entity);
    void RemoveEntity(Entity entity);
    [[nodiscard]] const List<Entity> &GetEntities() const { return entities; }
    [[nodiscard]] const Signature &GetSignature() const { return componentsSignature; }

    template<typename T>
    void RequireComponent();
};

template<typename T>
void System::RequireComponent() {
    const auto componentId = Component<T>::GetId();
    componentsSignature.set(componentId);
}


///////////////////////////////////////////////////////////////////////////////////////
/// Pool
///////////////////////////////////////////////////////////////////////////////////////
class BasePool {
public:
    virtual ~BasePool() = default;
};

template<typename T>
class Pool final : BasePool {
    List<T> components;

public:
    explicit Pool(const int size = 100) { Resize(size); }
    ~Pool() override = default;

    [[nodiscard]] bool IsEmpty() const { return components.empty(); }
    [[nodiscard]] int GetSize() const { return components.size(); }
    void Resize(int size) { components.resize(size); }
    void Clear() { components.clear(); }
    void Add(T component) { components.push_back(component); }
    void Set(int index, T component) { components[index] = component; }
    T &Get(int index) { return components[index]; }
    T &operator[](int index) { return components[index]; }
};

///////////////////////////////////////////////////////////////////////////////////////
/// Registry
///////////////////////////////////////////////////////////////////////////////////////
class Registry {
    int numEntities{0};
    List<Shared<BasePool>> componentPools{};
    List<Signature> entityComponentSignatures{};
    Dictionary<std::type_index, Shared<System>> systems{};
    Set<Entity> addedEntities{};

public:
    void Update();
    Entity CreateEntity();
    void AddEntityToSystems(Entity entity) const;
    void DestroyEntity(Entity entity);

    template<typename T, typename... Args>
    void AddComponent(Entity entity, Args &&... args);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    bool HasComponent(Entity entity);

    template<typename T, typename... Args>
    void AddSystem(Args &&... args);

    template<typename T>
    void RemoveSystem();

    template<typename T>
    bool HasSystem() const;

    template<typename T>
    T &GetSystem() const;
};

template<typename T, typename... Args>
void Registry::AddComponent(const Entity entity, Args &&... args) {
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) componentPools.resize(componentId + 1, nullptr);
    if (!componentPools[componentId]) {
        Shared<BasePool> pool = std::make_shared<Pool<T> >(componentId);

        componentPools[componentId] = pool;
    }

    auto pool = componentPools[componentId];

    if (entityId >= pool->GetSize()) {
        pool->resize(numEntities);
    }
    pool->Set(entityId, std::forward<Args>(args)...);
    entityComponentSignatures[entityId].set(componentId);
}

template<typename T>
void Registry::RemoveComponent(const Entity entity) {
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
}

template<typename T>
bool Registry::HasComponent(const Entity entity) {
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

template<typename T, typename... Args>
void Registry::AddSystem(Args &&... args) {
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    systems.insert({std::type_index(typeid(T)), system});
}

template<typename T>
void Registry::RemoveSystem() {
    systems.erase(std::type_index(typeid(T)));
}

template<typename T>
bool Registry::HasSystem() const {
    return systems.contains(std::type_index(typeid(T)));
}

template<typename T>
T &Registry::GetSystem() const {
    return systems.at(std::type_index(typeid(T)));
}
