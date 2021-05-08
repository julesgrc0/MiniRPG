#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

typedef struct MouseUpdate
{
	bool isOnMap = false;
	bool hasMove = false;
	bool hasClick = false;
	sf::Vector2f coord = sf::Vector2f(0, 0);
} MouseUpdate;

class Mouse
{
public:
	Mouse();
	~Mouse();
	MouseUpdate Update(sf::RenderWindow &);
	void Draw(sf::RenderWindow &);
	void Draw(sf::RenderTexture &);
	int size = 50;

private:
	sf::Vector2f m_pos = sf::Vector2f(50, 50);
	bool isPress;
};
