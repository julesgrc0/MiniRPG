#pragma once
#include <SFML/Graphics.hpp>
#include"JsonMapReader.h"
#include "GameTexture.h"

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
	bool isDirectionR = false;
	bool isCTRL = false;

	PlayerStates activeState = STATIC_L;
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

	bool getTexture(PlayerStates, GameTexture&, sf::Texture*&);

	bool isNew = false;
	const int Max_player_speed = 5;
	int playerSeep = Max_player_speed;
	int playerXP = 0;
	float animation = 0.0f;
	bool isAnimationActive = false;

	std::vector<sf::Vector2f> visitedChunks;
	Chunk* getChunk(std::vector<Chunk*>& ,sf::Vector2f);
};
