#include "game.h"

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

DestroedBlock::DestroedBlock()
{
    color = sf::Color::Black;
    isDestroed = 1;
    colorIsChecked = 0;
    needToCheck = 0;
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
        drop(element);

    blocks[start.x][start.y].needToCheck = 1;
}

bool Field::needToCheck(sf::Vector2i block)
{
    if (!isBlockInField(block))
        return 0;

    bool result = blocks[block.x][block.y].needToCheck;
    blocks[block.x][block.y].needToCheck = 0;
    return result;
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

Game::Game()
{
    score = 0;
    selected_block = sf::Vector2i(-1, -1);
    font.openFromFile(FONT_FILE);
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

void Game::resetSelectedBlock()
{
    field.clickBlock(selected_block.x, selected_block.y);
    selected_block = sf::Vector2i(-1, -1);
}

void Game::selectBlock(sf::Vector2i mousePosition)
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

void Game::makeMove(sf::Vector2i blockA, sf::Vector2i blockB)
{
    field.flipBlocks(blockA, blockB);
    checkBlock(blockA);
    checkBlock(blockB);
}

void Game::checkBlock(sf::Vector2i block)
{
    int withSameColor = field.sameColorCount(block);

    if (withSameColor >= DESTROY_AMOUNT)
    {
        field.destroy(block);

        score += withSameColor;
        needToDrop.push_back(block);
    }
}

bool Game::canDrop()
{
    return needToDrop.size() != 0;
}

void Game::dropAll()
{
    for (auto element : needToDrop)
    {
        field.drop(element);
        needToCheck.push_back(element);
    }
    needToDrop.clear();
}

void Game::checkAll()
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
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    sf::Text text(font, "Score: " + std::to_string(score));
    text.setFillColor(sf::Color::White);
    target.draw(text, states);
    states.transform.translate(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    target.draw(field, states);
}