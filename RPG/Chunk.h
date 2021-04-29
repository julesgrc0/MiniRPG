#pragma once
#include "Case.h"
#include <SFML/Graphics.hpp>
#include "GameTexture.h"

enum ItemTypes
{
	SWORD,
	ITEM_NONE
};

enum EnemiTypes
{
	GOBLIN,
	ENIMIE_NONE
};

class Chunk
{
public:
	Chunk();
	~Chunk();
	std::vector<Case *> chunk;
	sf::Vector2f position;
	int  size = 50;
	void Draw(sf::RenderWindow&, GameTexture&);
	void Draw(sf::RenderTexture&, GameTexture&);
	std::vector<std::pair<sf::Vector2f, EnemiTypes>> listEnemies;
	std::vector<std::pair<sf::Vector2f, ItemTypes>> listItems;
};
