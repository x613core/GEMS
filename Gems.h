#pragma once
#include "game.h"

#ifdef FIELD

class Gems
{
protected:
    Field field;
    sf::Vector2i selected_block;
    int score;
    sf::Font font;
    std::list<sf::Vector2i> needToDrop;
    std::list<sf::Vector2i> needToCheck;

    void resetSelectedBlock();
    void makeMove(sf::Vector2i blockA, sf::Vector2i blockB);
    void checkBlock(sf::Vector2i block);
    bool canDrop();
    bool canCheck();
    void dropAll();
    void checkAll();

public:
    sf::RenderWindow window;

    Gems();

    void draw();
    void update();
    void init();

    void displayWithDelay(int deleyMiliseconds);
    void selectBlock(sf::Vector2i mousePosition);
};

#endif
