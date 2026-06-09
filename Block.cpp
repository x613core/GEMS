#include "Block.h"

sf::Color getRandomColor()
{
    int rand_index = rand() % COLORS_COUNT;
    return COLORS[rand_index];
}

Block::Block()
{
    isSelected = 0;
    colorIsChecked = 0;
    needToCheck = 0;
    isDestroed = 0;
    color = getRandomColor();
}

void Block::click()
{
    isSelected = !isSelected;
}

bool Block::sameColor(Block block)
{
    return block.color == color;
}

void Block::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 2 * FRAME_SIZE, BLOCK_SIZE - 2 * FRAME_SIZE));
    block.setFillColor(color);

    block.setOutlineColor(color);
    if (isSelected)
        block.setOutlineColor(HIGHLIGHT_COLOR);
    block.setOutlineThickness(FRAME_SIZE);

    target.draw(block, states);
}