#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <random>
#include <SFML/Graphics.hpp>

#define FONT_FILE "Samson.ttf"
#define BOMB_SPRITE "bomb_circle.png"

constexpr int FIELD_SIZE = 5;   // bloks amount in field
constexpr int BLOCK_SIZE = 100; // block size in pixels

constexpr int WINDOW_SIZE = 900; // window size

constexpr int DELTA_TIME = 300; // time of dropping blocks in milliseconds

constexpr int DESTROY_AMOUNT = 3; // amount of blocks with same color to destroy them

constexpr int FRAME_SIZE = 10;                                  // size of frame to highlight selected block
constexpr sf::Color HIGHLIGHT_COLOR = sf::Color(128, 128, 128); // color to highlight selected block

constexpr int COLORS_COUNT = 7;
constexpr sf::Color COLORS[COLORS_COUNT] = {
    sf::Color::Cyan,
    sf::Color::Green,
    sf::Color::Magenta,
    sf::Color::Yellow,
    sf::Color::Blue,
    sf::Color::White,
    sf::Color::Red}; // possible colors for blocks

#include "Block.h"
#include "DestroedBlock.h"
#include "Bonus.h"
#include "Field.h"
#include "Gems.h"