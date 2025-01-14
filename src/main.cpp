#include <SDL2/SDL_main.h>
#include <sol/sol.hpp>
#include "core/Game.h"


void TestLua() {
    sol::state lua;
    LOG("TestLua");

    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/test.lua");

    int luaVariable = lua["some_variable"];

    LOG("Value of lua variable gets: {}", luaVariable);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    // Game game;
    //
    // game.Initialize();
    // game.Run();
    // game.Destroy();

    TestLua();

    return 0;
}
