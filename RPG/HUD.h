#pragma once
#include<SFML/Graphics.hpp>
#include "Utils.h"

struct HUDupdateObject
{
	sf::Vector2f chunkPos = sf::Vector2f(0,0);
	sf::Vector2f playerPos = sf::Vector2f(0,0);
	int enemiesCount = 0;
	int inventoryItemsCount = 0;
	ItemTypes activeBar[8];
	int activeBarIndex = 0;
	int playerLife = 0;
	int playerXP = 0;
	int playerKill = 0;
	float deltatime = 0.0f;
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

