#include "Gems.h"

Gems::Gems(sf::RenderWindow *window)
{
    score = 0;
    newClick = 0;
    this->window = window;
    lastClickPosition = sf::Vector2i(-1, -1);
    std::filesystem::path p = __FILE__;
    int isOpend = font.openFromFile(p.parent_path().string() + "/" FONT_FILE);
}

void Gems::drawAndDisplay(bool needToAnimate)
{
    window->clear(sf::Color::Black);

    sf::Text text(font, "Score: " + std::to_string(score));
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(0, 0));
    window->draw(text);

    field.setPosition(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    if (needToAnimate)
        field.animateChange(window);
    else
        window->draw(field);

    window->display();
}

void Gems::update()
{
    if (newClick)
    {
        field.selectBlock(lastClickPosition);
        newClick = 0;
        drawAndDisplay();
    }

    while (field.canCheck() || field.canDrop())
    {
        score += field.checkAll();
        drawAndDisplay(1);
        field.dropAll();
        drawAndDisplay(1);
    }

    drawAndDisplay();
}

void Gems::run()
{
    while (window->isOpen())
    {
        while (const auto event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window->close();

            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent)
                setLastClickPos(mouseEvent->position);
        }

        update();
    }
}

void Gems::setLastClickPos(sf::Vector2i pos)
{
    newClick = 1;
    lastClickPosition = pos;
}