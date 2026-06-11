#include "Field.h"

Field::Field()
{
    selected_block = sf::Vector2i(-1, -1);
}

void Field::clickBlock(int i, int j)
{
    blocks[i][j].click();
}

void Field::flipBlocks(sf::Vector2i first, sf::Vector2i second)
{
    auto temporal = blocks[first.x][first.y];
    blocks[first.x][first.y] = blocks[second.x][second.y];
    blocks[second.x][second.y] = temporal;
}

int Field::sameColorCount(sf::Vector2i start)
{
    int result = 1;
    auto block = &blocks[start.x][start.y];
    block->colorIsChecked = 1;

    sf::Vector2i neightbours[4] = {
        sf::Vector2i(start.x - 1, start.y),
        sf::Vector2i(start.x + 1, start.y),
        sf::Vector2i(start.x, start.y - 1),
        sf::Vector2i(start.x, start.y + 1)

    };

    for (auto neightbour : neightbours)
    {
        Block current = blocks[neightbour.x][neightbour.y];
        if (isBlockInField(neightbour) && !current.colorIsChecked && block->sameColor(current))
            result += sameColorCount(neightbour);
    }

    block->colorIsChecked = 0;
    return result;
}

void Field::destroy(sf::Vector2i start)
{
    auto block = &blocks[start.x][start.y];
    block->colorIsChecked = 1;

    sf::Vector2i neightbours[4] = {
        sf::Vector2i(start.x - 1, start.y),
        sf::Vector2i(start.x + 1, start.y),
        sf::Vector2i(start.x, start.y - 1),
        sf::Vector2i(start.x, start.y + 1)

    };

    for (auto neightbour : neightbours)
    {
        Block current = blocks[neightbour.x][neightbour.y];
        if (isBlockInField(neightbour) && !current.colorIsChecked && block->sameColor(current))
            destroy(neightbour);
    }

    block->colorIsChecked = 0;
    Block destroedBlock = DestroedBlock();
    blocks[start.x][start.y] = destroedBlock;
    needToDrop.push_back(start);
}

void Field::drop(sf::Vector2i start)
{
    if (!isBlockInField(start))
        return;
    Block block = blocks[start.x][start.y];
    if (!block.isDestroed)
        return;

    sf::Vector2i top(start.x, start.y - 1);
    if (top.y < 0)
        blocks[start.x][start.y] = Block();
    else
    {
        drop(top);
        flipBlocks(top, start);
    }
    sf::Vector2i neightbours[4] = {
        top,
        sf::Vector2i(start.x - 1, start.y),
        sf::Vector2i(start.x + 1, start.y),
        sf::Vector2i(start.x, start.y + 1)

    };
    for (auto element : neightbours)
        if (isBlockInField(element))
            drop(element);

    blocks[start.x][start.y].needToCheck = 1;
}

int Field::checkBlock(sf::Vector2i block)
{
    int score = 0;
    if (!isBlockInField(block))
        return 0;

    int withSameColor = sameColorCount(block);

    if (withSameColor >= DESTROY_AMOUNT)
    {
        destroy(block);

        score += withSameColor;
        needToDrop.push_back(block);
    }
    return score;
}

bool Field::canDrop()
{
    return needToDrop.size() != 0;
}

bool Field::canCheck()
{
    return needToCheck.size() != 0;
}

void Field::dropAll()
{
    for (auto element : needToDrop)
    {
        drop(element);

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

int Field::checkAll()
{
    int score = 0;
    for (auto element : needToCheck)
        score += checkBlock(element);

    needToCheck.clear();
    return score;
}

void Field::resetSelectedBlock()
{
    if (selected_block != sf::Vector2i(-1, -1))
    {
        clickBlock(selected_block.x, selected_block.y);
        selected_block = sf::Vector2i(-1, -1);
    }
}

void Field::selectBlock(sf::Vector2i mousePosition)
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
            clickBlock(currentBlock.x, currentBlock.y);
        }
    }
}

void Field::makeMove(sf::Vector2i blockA, sf::Vector2i blockB)
{
    flipBlocks(blockA, blockB);
    needToCheck.push_back(blockA);
    needToCheck.push_back(blockB);
}

void Field::animateChange(sf::RenderWindow *window)
{
    window->draw(*this);
    std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
}

void Field::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    auto startTransform = states.transform;

    for (int i = 0; i < FIELD_SIZE; i++)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            Block current = blocks[i][j];
            states.transform.translate(sf::Vector2f(i * BLOCK_SIZE, j * BLOCK_SIZE));
            target.draw(current, states);
            states.transform = startTransform;
        }
    }
}

sf::Vector2i convertMousePosToBlock(sf::Vector2i mousePosition)
{
    int i = mousePosition.x / BLOCK_SIZE - 1;
    int j = mousePosition.y / BLOCK_SIZE - 1;

    return sf::Vector2i(i, j);
}

bool isBlockInField(sf::Vector2i blockPosition)
{
    return blockPosition.x >= 0 && blockPosition.x < FIELD_SIZE && blockPosition.y >= 0 && blockPosition.y < FIELD_SIZE;
}

bool areNeightbours(sf::Vector2i blockA, sf::Vector2i blockB)
{
    if (blockA == sf::Vector2i(-1, -1) || blockB == sf::Vector2i(-1, -1))
        return false;

    int x = blockA.x;
    int y = blockA.y;
    sf::Vector2i neightboursA[4] = {
        sf::Vector2i(x - 1, y),
        sf::Vector2i(x + 1, y),
        sf::Vector2i(x, y - 1),
        sf::Vector2i(x, y + 1)

    };

    for (auto neightbour : neightboursA)
        if (neightbour == blockB)
            return true;

    return false;
}