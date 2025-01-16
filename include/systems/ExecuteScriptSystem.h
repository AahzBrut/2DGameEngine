#pragma once
#include "components/ScriptComponent.h"
#include "ecs/ECS.h"

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
void SetPosition(Entity entity, double x, double y) {
    if (!entity.HasComponent<TransformComponent>()) return;
    auto& transformComponent = entity.GetComponent<TransformComponent>();
    transformComponent.position.x = static_cast<float>(x);
    transformComponent.position.y = static_cast<float>(y);
    LOG("SetPosition");
}

class ExecuteScriptSystem : public System {
    public:
    explicit ExecuteScriptSystem(sol::state& lua) {
        RequireComponent<ScriptComponent>();
        lua.new_usertype<Entity>(
            "entity",
            "GetId", &Entity::GetId,
            "Destroy", &Entity::Destroy);
        lua.set_function("SetPosition", SetPosition);
    }

    void Update(const float deltaTime) const {
        for (auto entity: GetSystemEntities()) {
            const auto scriptComponent = entity.GetComponent<ScriptComponent>();
            LOG("Starting lua script execution.");
            scriptComponent.luaFunction(entity, deltaTime, 0);
            LOG("Finished lua script execution.");
        }
    }
};
