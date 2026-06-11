#pragma once
#include "game.h"

#define BLOCK

class Block : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Color color;
    bool isSelected;

public:
    Block();
    void click();
    bool colorIsChecked;
    bool needToCheck;
    bool isDestroed;
    bool sameColor(Block block);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};