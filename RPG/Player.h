#pragma once
#include <SFML/Graphics.hpp>
#include"JsonMapReader.h"
#include "GameTexture.h"

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

class Player
{
public:
	Player();
	~Player();
	
	bool KeyBoardUpdate(float deltatime, std::vector<Case*>& chunk);
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

	PlayerStates activeState = STATIC_L;

	bool getTexture(PlayerStates, GameTexture&, sf::Texture*&);

	bool isDirectionR = false;
	bool isNew = false;
	const int Max_player_speed = 5;
	int playerSeep = Max_player_speed;
	int playerXP = 0;
	float animation = 0.0f;
	bool isAnimationActive = false;

	std::vector<sf::Vector2f> visitedChunks;
	std::vector<Case*> getChunk(std::vector<Chunk*>& ,sf::Vector2f);
};
