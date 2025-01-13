#include <SDL2/SDL_main.h>
#include "core/Game.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
