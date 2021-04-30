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
	bool hasTexture = false;
	if (textures.block_textures.size() != 0)
	{
		for (CaseTypes type = GRASS; type != NONE; type = (CaseTypes)((int)type + 1))
		{
			if (this->case_type == type)
			{
				hasTexture = this->SetTexture(textures, (int)type);
				break;
			}
		}
	}

	if (!hasTexture)
	{
		this->DrawDefault();
	}

	sf::Sprite sprite(texture.getTexture());
	sprite.setPosition(pos);
	return sprite;
}

bool Case::SetTexture(GameTexture& textures,int id)
{
	sf::Texture* img = new sf::Texture();
	if (getTexture(textures.block_textures, id, img))
	{
		this->texture.clear();
		sf::Sprite sprite((*img));
		this->texture.draw(sprite);
		return true;
	}
	return false;
}

void Case::DrawDefault()
{
	switch (this->case_type)
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
	case GROUND:
		this->texture.clear(sf::Color::Color(67, 44, 21));
		break;
	default:
		this->texture.clear();
		break;
	}
}