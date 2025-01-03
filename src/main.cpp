#include "core/Game.h"

#include <SDL2/SDL.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    SDL_Quit();
    return 0;
}
