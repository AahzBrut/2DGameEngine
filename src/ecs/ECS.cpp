#include "ecs/ECS.h"

#include <ranges>


#include "core/Logger.h"


int BaseComponent::nextId = 0;

void System::AddEntity(const Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntity(const Entity entity) {
    std::erase_if(entities, [&entity](const Entity &other) { return entity == other; });
}

void Registry::Update() {
    for (const auto entity: addedEntities) {
        AddEntityToSystems(entity);
    }
    addedEntities.clear();

}

Entity Registry::CreateEntity() {
    const auto entityId = numEntities++;
    const auto entity = Entity(entityId);
    addedEntities.insert(entity);
    LOG("Entity added: {:d}", entityId);

    if (entityId>=entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }

    return entity;
}

void Registry::AddEntityToSystems(const Entity entity) const {
    const auto entityId = entity.GetId();
    const auto& signature = entityComponentSignatures[entityId];
    for (const auto &system : std::ranges::views::values(systems)) {
        if (const auto systemComponentSignature = system->GetSignature();
            (systemComponentSignature & signature) == systemComponentSignature) {
            system->AddEntity(entity);
        }
    }

}

void Registry::DestroyEntity(Entity entity) {}
