#pragma once
#include "game.h"

#ifdef BLOCK
#define FIELD

class Field : public sf::Drawable, public sf::Transformable
{
protected:
    Block blocks[FIELD_SIZE][FIELD_SIZE];
    sf::Vector2i selected_block;
    std::list<sf::Vector2i> needToDrop;
    std::list<sf::Vector2i> needToCheck;

public:
    Field();

    void clickBlock(int i, int j);
    void selectBlock(sf::Vector2i mousePosition);
    void resetSelectedBlock();

    void flipBlocks(sf::Vector2i first, sf::Vector2i second);
    void makeMove(sf::Vector2i blockA, sf::Vector2i blockB);

    int sameColorCount(sf::Vector2i start);
    void destroy(sf::Vector2i start);
    void drop(sf::Vector2i start);
    int checkBlock(sf::Vector2i block);

    bool canDrop();
    bool canCheck();
    void dropAll();
    int checkAll();

    void animateChange(sf::RenderWindow *window);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

sf::Vector2i convertMousePosToBlock(sf::Vector2i mousePosition);
bool isBlockInField(sf::Vector2i blockPosition);
bool areNeightbours(sf::Vector2i blockA, sf::Vector2i blockB);
#endif