#pragma once
#include "game.h"

#ifdef FIELD

class Gems
{
protected:
    Field field;
    int score;
    sf::Font font;
    sf::Vector2i lastClickPosition;
    bool newClick;

    sf::RenderWindow *window;

public:
    Gems(sf::RenderWindow *window);

    void drawAndDisplay(bool needToAnimate = 0);
    void update();
    void run();
    void setLastClickPos(sf::Vector2i pos);
};

#endif
