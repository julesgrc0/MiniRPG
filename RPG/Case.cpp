#include <SFML/Graphics.hpp>
#include "Case.h"
#include "Utils.h"
#include <iostream>

Case::Case(int size)
{
	this->texture.create(size,size);
}

Case::~Case()
{
}

void Case::Update(float deltatime)
{
}

sf::Sprite Case::Draw(sf::Vector2f pos, GameTexture& textures)
{
	this->DrawDefault();

	if (textures.block_textures.size() != 0)
	{
		switch (this->type)
		{
		case GRASS:
			this->SetTexture(textures, 1);
			break;
		case SAND:
			this->SetTexture(textures, 2);
			break;
		case WATER:
			this->SetTexture(textures, 3);
			break;
		}
	}

	sf::Sprite sprite(texture.getTexture());
	sprite.setPosition(pos);
	return sprite;
}

void Case::SetTexture(GameTexture& textures,int id)
{
	sf::Texture* img = new sf::Texture();
	if (getTexture(textures.block_textures, id, img))
	{
		this->texture.clear();
		sf::Sprite sprite((*img));
		sprite.setPosition(sf::Vector2f(0, 0));
		this->texture.draw(sprite);
	}
}

void Case::DrawDefault()
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
}