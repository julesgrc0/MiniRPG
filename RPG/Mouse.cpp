#include "Mouse.h"

Mouse::Mouse()
{
}

bool Mouse::Update(sf::RenderWindow &window)
{
    bool mouseUpdate = false;

    sf::Vector2f last_m_pos = this->m_pos;
    this->m_pos = sf::Vector2f(sf::Mouse::getPosition(window));
    this->m_pos = sf::Vector2f((int)this->m_pos.x / 50 * 50, (int)this->m_pos.y / 50 * 50);

    if (last_m_pos.x != this->m_pos.x || last_m_pos.y != this->m_pos.y)
    {
        mouseUpdate = true;
    }

    return mouseUpdate;
}

void Mouse::Draw(sf::RenderWindow &window)
{
    sf::RectangleShape mouseRect;
    mouseRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
    mouseRect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
    mouseRect.setOutlineThickness(1.0f);
    mouseRect.setPosition(this->m_pos);
    mouseRect.setSize(sf::Vector2f(50, 50));

    window.draw(mouseRect);
}

void Mouse::Draw(sf::RenderTexture &texture)
{
    sf::RectangleShape mouseRect;
    mouseRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
    mouseRect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
    mouseRect.setOutlineThickness(1.0f);
    mouseRect.setPosition(this->m_pos);
    mouseRect.setSize(sf::Vector2f(50, 50));

    texture.draw(mouseRect);
}

Mouse::~Mouse()
{
}