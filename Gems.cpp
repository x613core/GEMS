#include "Gems.h"

Gems::Gems()
{
    score = 0;
    selected_block = sf::Vector2i(-1, -1);
    font.openFromFile(FONT_FILE);

    _isChecked = 0;
    _isDroped = 1;
}

void Gems::draw()
{
    window.clear(sf::Color::Black);

    sf::Text text(font, "Score: " + std::to_string(score));
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(0, 0));
    window.draw(text);

    field.setPosition(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    window.draw(field);
}

void Gems::update()
{
    if (!_isDroped)
    {
        dropAll();

        _isChecked = 0;
        _isDroped = 1;
    }
    else if (!_isChecked)
    {
        checkAll();

        _isDroped = 0;
        _isChecked = 1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
}

void Gems::init()
{
    window.create(sf::VideoMode(sf::Vector2u(900, 900)), "GEMS");
}

void Gems::resetSelectedBlock()
{
    field.clickBlock(selected_block.x, selected_block.y);
    selected_block = sf::Vector2i(-1, -1);
}

void Gems::selectBlock(sf::Vector2i mousePosition)
{
    auto currentBlock = convertMousePosToBlock(mousePosition);

    if (isBlockInField(currentBlock))
    {
        auto selectedBlock = selected_block;
        resetSelectedBlock();
        if (areNeightbours(selectedBlock, currentBlock))
            makeMove(selectedBlock, currentBlock);
        else if (selectedBlock != currentBlock)
        {
            selected_block = currentBlock;
            field.clickBlock(currentBlock.x, currentBlock.y);
        }
    }
}

void Gems::makeMove(sf::Vector2i blockA, sf::Vector2i blockB)
{
    field.flipBlocks(blockA, blockB);
    needToCheck.push_back(blockA);
    needToCheck.push_back(blockB);
    // checkBlock(blockA);
    // checkBlock(blockB);
}

void Gems::checkBlock(sf::Vector2i block)
{
    if (!isBlockInField(block))
        return;

    int withSameColor = field.sameColorCount(block);

    if (withSameColor >= DESTROY_AMOUNT)
    {
        field.destroy(block);

        score += withSameColor;
        needToDrop.push_back(block);
    }
}

bool Gems::canDrop()
{
    return needToDrop.size() != 0;
}

bool Gems::canCheck()
{
    return needToCheck.size() != 0;
}

void Gems::dropAll()
{
    for (auto element : needToDrop)
    {
        field.drop(element);
        needToCheck.push_back(element);
    }
    needToDrop.clear();
}

void Gems::checkAll()
{
    for (auto element : needToCheck)
    {
        checkBlock(element);
        sf::Vector2i neightbours[4] = {
            sf::Vector2i(element.x - 1, element.y),
            sf::Vector2i(element.x + 1, element.y),
            sf::Vector2i(element.x, element.y - 1),
            sf::Vector2i(element.x, element.y + 1)};

        for (auto neightbour : neightbours)
            if (field.needToCheck(neightbour))
                checkBlock(neightbour);
    }

    needToCheck.clear();
}
