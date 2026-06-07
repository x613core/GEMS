#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#define FONT_FILE "Samson.ttf"
#define BOMB_SPRITE "bomb_circle.png"

const int FIELD_SIZE = 5;   // bloks amount in field
const int BLOCK_SIZE = 100; // block size in pixels

const int DELTA_TIME = 300; // time of dropping blocks in milliseconds

const int DESTROY_AMOUNT = 3; // amount of blocks with same color to destroy them

const int FRAME_SIZE = 10;                                  // size of frame to highlight selected block
const sf::Color HIGHLIGHT_COLOR = sf::Color(128, 128, 128); // color to highlight selected block

const int COLORS_COUNT = 7;
const sf::Color COLORS[COLORS_COUNT] = {
    sf::Color::Cyan,
    sf::Color::Green,
    sf::Color::Magenta,
    sf::Color::Yellow,
    sf::Color::Blue,
    sf::Color::White,
    sf::Color::Red}; // possible colors for blocks

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

class DestroedBlock : public Block
{
public:
    DestroedBlock();
};

class Field : public sf::Drawable,
              public sf::Transformable
{
protected:
    Block blocks[FIELD_SIZE][FIELD_SIZE];

public:
    void clickBlock(int i, int j);
    void flipBlocks(sf::Vector2i first, sf::Vector2i second);
    int sameColorCount(sf::Vector2i start);
    void destroy(sf::Vector2i start);
    void drop(sf::Vector2i start);
    bool needToCheck(sf::Vector2i block);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

class Game : public sf::Drawable, public sf::Transformable
{
protected:
    Field field;
    sf::Vector2i selected_block;
    int score;
    sf::Font font;
    std::list<sf::Vector2i> needToDrop;
    std::list<sf::Vector2i> needToCheck;

public:
    Game();
    void resetSelectedBlock();
    void selectBlock(sf::Vector2i mousePosition);
    void makeMove(sf::Vector2i blockA, sf::Vector2i blockB);
    void checkBlock(sf::Vector2i block);
    bool canDrop();
    void dropAll();
    void checkAll();
    void virtual draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

sf::Vector2i convertMousePosToBlock(sf::Vector2i mousePosition);
bool isBlockInField(sf::Vector2i blockPosition);
bool areNeightbours(sf::Vector2i blockA, sf::Vector2i blockB);