#pragma once
#include "game.h"

#ifdef BLOCK
#define FIELD

class Field : public sf::Drawable, public sf::Transformable
{
protected:
    Block blocks[FIELD_SIZE][FIELD_SIZE];

public:
    void clickBlock(int i, int j);
    void flipBlocks(sf::Vector2i first, sf::Vector2i second);
    int sameColorCount(sf::Vector2i start);
    void destroy(sf::Vector2i start, std::list<sf::Vector2i> *needToDrop);
    void drop(sf::Vector2i start);
    bool needToCheck(sf::Vector2i block);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

sf::Vector2i convertMousePosToBlock(sf::Vector2i mousePosition);
bool isBlockInField(sf::Vector2i blockPosition);
bool areNeightbours(sf::Vector2i blockA, sf::Vector2i blockB);
#endif