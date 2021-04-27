#include <SFML/Graphics.hpp>
#include "Case.h"
#include <iostream>

Case::Case()
{
	this->texture.create(50, 50);
}

Case::~Case()
{
}

void Case::Update(float deltatime)
{
}

sf::Sprite Case::Draw(sf::Vector2f pos)
{
	switch (this->type)
	{
	case GRASS:
		this->texture.clear(sf::Color::Color(68, 204, 14, 255));
		break;
	case SAND:
		this->texture.clear(sf::Color::Color(219, 169, 31, 255));
		break;
	case WATER:
		this->texture.clear(sf::Color::Color(31, 169, 219, 255));
		break;
	default:
		this->texture.clear();
		break;
	}
	sf::Sprite sprite(texture.getTexture());
	sprite.setPosition(pos);
	return sprite;
}
