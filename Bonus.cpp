#include "Bonus.h"

void Bonus::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 2 * FRAME_SIZE, BLOCK_SIZE - 2 * FRAME_SIZE));
    block.setFillColor(color);

    block.setOutlineColor(color);
    if (isSelected)
        block.setOutlineColor(HIGHLIGHT_COLOR);
    block.setOutlineThickness(FRAME_SIZE);

    target.draw(block, states);

    sf::Sprite sprite(texture);
    sprite.setScale({0.3, 0.3});

    target.draw(sprite, states);
}