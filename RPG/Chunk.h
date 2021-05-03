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
	GOBLIN_S,
	GOBLIN_L,
	GOBLIN_R,
	ENIMIES_NONE
};

enum PlayerStates
{
	STATIC_R,
	STATIC_L,


	S_ATTACK_R,
	S_ATTACK_L,


	S_DEFEND_R,
	S_DEFEND_L,


	S_N_ATTACK,

	S_LIGHT,
	S_BAG,
	S_WATER,

	DEAD,
};

typedef struct ChunkUpdate{
	bool EnemyUpdate = false;
	
}ChunkUpdate;

typedef struct PlayerObject
{
	bool isRight;
	sf::Vector2f position;
	bool isCTRL;
	PlayerStates state;
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
	ChunkUpdate Update(float deltatime, PlayerObject& player);

	std::vector<std::pair<sf::Vector2f, EnemiTypes>> listEnemies;
	std::vector<std::pair<sf::Vector2f, ItemTypes>> listItems;
public:
	void DrawEnemies(std::pair<sf::Vector2f, EnemiTypes> enemy,sf::RenderTexture&, GameTexture&);
	float chunkTime;
};
