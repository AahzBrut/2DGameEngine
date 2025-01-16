#pragma once
#include <utility>
#include <sol/sol.hpp>

struct ScriptComponent {
    sol::function luaFunction;

    explicit ScriptComponent(sol::function luaFunction = sol::lua_nil) : luaFunction{std::move(luaFunction)} {}
};
