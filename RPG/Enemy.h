#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include "GameTexture.h"
#include "Utils.h"


class Enemy
{
public:
	EnemiTypes type;
	sf::Vector2f position;

	void Draw(sf::RenderTexture&, GameTexture&);
	EnemyUpdate Update(float&, PlayerObject&,std::vector<Case*>&);

	int life = 100;
	float damage = 0.5;
private:
	int actualDist = 0;
	float time = 0;
	int size = 50;
	bool isdetected = false;
};

