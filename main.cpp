#include "game.h"

int main()
{
    Gems gems;
    gems.init();

    while (gems.window.isOpen())
    {
        while (const auto event = gems.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                gems.window.close();

            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent)
                gems.selectBlock(mouseEvent->position);
        }

        gems.update();
    }
    return 0;
}
