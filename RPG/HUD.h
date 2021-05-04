#pragma once
#include<SFML/Graphics.hpp>
#include "Utils.h"

struct HUDupdateObject
{
	sf::Vector2f chunkPos;
	sf::Vector2f playerPos;
	int enemiesCount;
	int inventoryItemsCount;
	ItemTypes activeBar[8];
	int activeBarIndex;
	int playerLife;
	int playerXP;
	int playerKill;
	float deltatime;
	int night;
};

class HUD
{
public:
	HUD(sf::Font&);
	~HUD();

	void Draw(sf::RenderWindow&,sf::Sprite&,GameTexture&);
	void Update(HUDupdateObject&);
private:
	HUDupdateObject active;
	sf::Font font;
};

