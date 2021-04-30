#pragma once
#include <SFML/Graphics.hpp>
#include"JsonMapReader.h"
#include "GameTexture.h"

enum PlayerStates
{
	STATIC,
	C_ARM,
	C_ATTACK,

	STATIC_SHIRT,
	H_N_ATTACK,
	H_ATTACK,

	STATIC__STUFF_R,
	STATIC_STUFF,
	S_N_ATTACK,
	S_ATTACK_1,
	S_ATTACK_2,
	S_ATTACK_3,
	S_ATTACK,
	S_DEFEND,
	S_LIGHT,
	S_BAG,
	S_WATER,

	DEAD,
};

class Player
{
public:
	Player();
	~Player();
	
	bool KeyBoardUpdate(float deltatime);
	bool MapUpdate(float deltatime,std::vector<Chunk*>&, Chunk*& activeChunk);
	bool isNewChunk();
	void addVisitedChunk(sf::Vector2f);

	void Draw(sf::RenderWindow &,GameTexture&);
	void Draw(sf::RenderTexture &,GameTexture&);

	bool isCollisionBlock(CaseTypes&);
	void goForward();

	sf::Vector2f playerPos;
	
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
	bool isNew = false;
	const int Max_player_speed = 5;
	int playerSeep = Max_player_speed;

	std::vector<sf::Vector2f> visitedChunks;
	std::vector<Case*> getChunk(std::vector<Chunk*>& ,sf::Vector2f);
};
