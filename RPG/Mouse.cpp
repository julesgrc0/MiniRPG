#include "Mouse.h"



Mouse::Mouse()
{
}

MouseUpdate Mouse::Update(sf::RenderWindow &window)
{
    MouseUpdate update;
    sf::Vector2f last_m_pos = this->m_pos;
    this->m_pos = sf::Vector2f(sf::Mouse::getPosition(window));
    this->m_pos = sf::Vector2f((int)this->m_pos.x / 50 * 50, (int)this->m_pos.y / 50 * 50);

    update.isOnMap = true;
    if (m_pos.x < 0 || m_pos.x >= this->size * 10)
    {
        update.isOnMap = false;
    }

    if (m_pos.y < 0 || m_pos.y >= this->size * 10)
    {
        update.isOnMap = false;
    }


    if (last_m_pos.x != this->m_pos.x || last_m_pos.y != this->m_pos.y)
    {
        update.hasMove = true;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        update.hasClick = true;
    }

    update.coord = m_pos;

    return update;
}

void Mouse::Draw(sf::RenderWindow &window)
{
    sf::RectangleShape mouseRect;
    mouseRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
    mouseRect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
    mouseRect.setOutlineThickness(1.0f);
    mouseRect.setPosition(this->m_pos);
    mouseRect.setSize(sf::Vector2f(this->size, this->size));

    window.draw(mouseRect);
}

void Mouse::Draw(sf::RenderTexture &texture)
{
    sf::RectangleShape mouseRect;
    mouseRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
    mouseRect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
    mouseRect.setOutlineThickness(1.0f);
    mouseRect.setPosition(this->m_pos);
    mouseRect.setSize(sf::Vector2f(this->size, this->size));

    texture.draw(mouseRect);
}

Mouse::~Mouse()
{
 
}