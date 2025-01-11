#include "ecs/ECS.h"
#include <algorithm>
#include <ranges>

#include "core/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const {
    return id;
}

void Entity::Destroy() const {
    registry->DestroyEntity(*this);
}

Entity& Entity::Tag(const std::string &tag) {
    registry->TagEntity(*this, tag);
    return *this;
}

bool Entity::HasTag(const std::string &tag) const {
    return registry->EntityHasTag(*this, tag);
}

Entity& Entity::Group(const std::string &group) {
    registry->GroupEntity(*this, group);
    return *this;
}

bool Entity::BelongsToGroup(const std::string &group) const {
    return registry->EntityBelongsToGroup(*this, group);
}

void System::AddEntityToSystem(const Entity &entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    std::erase_if(entities, [&entity](const Entity &other) {
        return entity == other;
    });
}

std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature &System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId;

    if (freeIds.empty()) {
        entityId = numEntities++;
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    return entity;
}

void Registry::DestroyEntity(const Entity &entity) {
    entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(const Entity &entity) const {
    const auto entityId = entity.GetId();

    const auto &entityComponentSignature = entityComponentSignatures[entityId];

    for (auto &system: std::ranges::views::values(systems)) {
        if (const auto &systemComponentSignature = system->GetComponentSignature();
            (entityComponentSignature & systemComponentSignature) == systemComponentSignature) {
            system->AddEntityToSystem(entity);
        }
    }
}

void Registry::RemoveEntityFromSystems(const Entity &entity) const {
    for (const auto &val: systems | std::views::values) {
        val->RemoveEntityFromSystem(entity);
    }
}

void Registry::TagEntity(Entity entity, const std::string &tag) {
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(const Entity &entity, const std::string &tag) const {
    if (!tagPerEntity.contains(entity.GetId())) {
        return false;
    }
    return entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string &tag) const {
    return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(const Entity &entity) {
    if (const auto taggedEntity = tagPerEntity.find(entity.GetId()); taggedEntity != tagPerEntity.end()) {
        const auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

void Registry::GroupEntity(Entity entity, const std::string &group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityBelongsToGroup(const Entity &entity, const std::string &group) const {
    if (!entitiesPerGroup.contains(group)) return false;
    const auto groupEntities = entitiesPerGroup.at(group);
    return groupEntities.contains(entity);
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string &group) const {
    auto &setOfEntities = entitiesPerGroup.at(group);
    return {setOfEntities.begin(), setOfEntities.end()};
}

void Registry::RemoveEntityGroup(const Entity &entity) {
    if (const auto groupedEntity = groupPerEntity.find(entity.GetId()); groupedEntity != groupPerEntity.end()) {
        if (const auto group = entitiesPerGroup.find(groupedEntity->second); group != entitiesPerGroup.end()) {
            if (const auto entityInGroup = group->second.find(entity); entityInGroup != group->second.end()) {
                group->second.erase(entityInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}

void Registry::Update() {
    for (const auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();

        for (const auto &pool: componentPools) {
            if (pool) pool->RemoveEntityFromPool(entity.GetId());
        }

        freeIds.push_back(entity.GetId());

        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
    }
    entitiesToBeKilled.clear();
}
