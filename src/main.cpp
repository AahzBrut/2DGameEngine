#include <SDL2/SDL_main.h>
#include "core/Game.h"

int main(int argc, char *argv[]) {
    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
