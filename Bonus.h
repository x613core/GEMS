#pragma once
#include "game.h"

#ifdef BLOCK
#define BONUS
class Bonus : public Block
{
protected:
    void (*effect)();
    sf::Texture texture;

public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
#endif