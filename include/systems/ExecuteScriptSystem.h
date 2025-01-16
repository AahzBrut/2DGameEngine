#pragma once
#include "components/ScriptComponent.h"
#include "ecs/ECS.h"


class ExecuteScriptSystem : public System {
    public:
    ExecuteScriptSystem() {
        RequireComponent<ScriptComponent>();
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
