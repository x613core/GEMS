#include "game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_SIZE, WINDOW_SIZE)), "GEMS");

    Gems gems(&window);
    gems.run();

    return 0;
}
