#include "game.h"

#define MAKE_FRAME_STEP                                                 \
    std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME)); \
    window.clear();                                                     \
    window.draw(game);                                                  \
    window.display();

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(900, 900)), "GEMS");
    Game game;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent)
                game.selectBlock(mouseEvent->position);
        }

        window.clear();
        window.draw(game);
        window.display();
        while (game.canDrop())
        {
            game.dropAll();
            MAKE_FRAME_STEP
            game.checkAll();
            MAKE_FRAME_STEP
        }
    }
    return 0;
}
