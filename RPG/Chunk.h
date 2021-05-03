#pragma once
#include "Case.h"
#include <SFML/Graphics.hpp>
#include "GameTexture.h"
#include "Utils.h"
#include "Enemy.h"

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
	ChunkUpdate Update(float deltatime, PlayerObject& player,std::vector<Case*>&);

	std::vector<Enemy*> listEnemies;
	std::vector<std::pair<sf::Vector2f, ItemTypes>> listItems;
public:
	float chunkTime;
	bool isNight = false;
};
