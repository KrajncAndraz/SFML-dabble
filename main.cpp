#include "Game.h"

#include <SFML/Graphics.hpp>

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    Game game;

    while (game.running())
    {
        game.update();
        game.render();
    }

    return 0;
}