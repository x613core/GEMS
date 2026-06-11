#include "Gems.h"

Gems::Gems()
{
    score = 0;
    selected_block = sf::Vector2i(-1, -1);
    std::filesystem::path p = __FILE__;
    int isOpend = font.openFromFile(p.parent_path().string() + "/" FONT_FILE);
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
    while (canCheck() || canDrop())
    {
        checkAll();
        displayWithDelay(DELTA_TIME);
        dropAll();
        displayWithDelay(DELTA_TIME);
    }

    displayWithDelay(0);
}

void Gems::init()
{
    window.create(sf::VideoMode(sf::Vector2u(900, 900)), "GEMS");
}

void Gems::displayWithDelay(int deleyMiliseconds)
{
    draw();
    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds(deleyMiliseconds));
}

void Gems::resetSelectedBlock()
{
    if (selectBlock != sf::Vector2i(-1, -1))
    {
        field.clickBlock(selected_block.x, selected_block.y);
        selected_block = sf::Vector2i(-1, -1);
    }
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
}

void Gems::checkBlock(sf::Vector2i block)
{
    if (!isBlockInField(block))
        return;

    int withSameColor = field.sameColorCount(block);

    if (withSameColor >= DESTROY_AMOUNT)
    {
        field.destroy(block, &needToDrop);

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

        while (isBlockInField(element))
        {
            auto i = std::find(needToCheck.begin(), needToCheck.end(), element);
            if (i == needToCheck.end())
                needToCheck.push_back(element);

            element.y--;
        }
    }
    needToDrop.clear();
}

void Gems::checkAll()
{
    for (auto element : needToCheck)
        checkBlock(element);

    needToCheck.clear();
}
