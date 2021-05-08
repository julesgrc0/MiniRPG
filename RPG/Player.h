#pragma once
#include <SFML/Graphics.hpp>
#include "JsonMapReader.h"
#include "GameTexture.h"
#include "Utils.h"
#include "GameAudio.h"

struct PlayerUpdate
{
	bool hasUpdate = false;
	bool hasChunkUpdate = false;
	bool isDead = false;
	Chunk *chunkUpdate;
};

class Player
{
public:
	Player();
	~Player();

	PlayerUpdate KeyBoardUpdate(float deltatime, Chunk *&chunk, bool night, GameAudio &);
	bool MapUpdate(float deltatime, std::vector<Chunk *> &, Chunk *&activeChunk, GameAudio &);
	bool isNewChunk();
	void addVisitedChunk(sf::Vector2f);

	void Draw(sf::RenderWindow &, GameTexture &);
	void Draw(sf::RenderTexture &, GameTexture &);

	bool isCollisionBlock(CaseTypes &);
	void goForward();

	sf::Vector2f playerPos;
	bool isDirectionR = false;
	bool isCTRL = false;
	int life = 100;
	int xp = 0;
	int kill = 0;

	int HUDbarIndex = 0;

	PlayerStates activeState = STATIC_L;

	std::vector<Item *> inventory;
	ItemTypes activeBar[8];

private:
	const CaseTypes collisions[8] = {
		CASTLE_WALL_L,
		CASTLE_WALL_R,
		CASTLE_WALL_T,
		CASTLE_WALL_B,
		CASTLE_WALL_ANGLE_L,
		CASTLE_WALL_ANGLE_T,
		CASTLE_WALL_ANGLE_R,
		CASTLE_WALL_ANGLE_B,
	};

	sf::Vector2f lastPos;

	bool getTexture(PlayerStates, GameTexture &, sf::Texture *&);

	bool isNew = false;
	const int Max_player_speed = 5;
	int playerSeep = Max_player_speed;
	int playerXP = 0;
	float damageTime = 0.0f;
	float enemiesDmgTime = 0.0f;
	float rCTRLpressTime = 0.0f;
	const int damage = 0.3;

	std::vector<sf::Vector2f> visitedChunks;
	Chunk *getChunk(std::vector<Chunk *> &, sf::Vector2f);
};
