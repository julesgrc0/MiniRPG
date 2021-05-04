#pragma once
#include<SFML/Graphics.hpp>
#include "Utils.h"

class Item
{
public:
	Item();
	~Item();

	void Draw(sf::RenderTexture&,GameTexture&);
	ItemUpdate Update(float&,PlayerObject&);

	sf::Vector2f position;
	ItemTypes type;

	int damage = 0;
private:
	float itemTime;
};

