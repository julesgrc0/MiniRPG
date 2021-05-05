#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include "GameTexture.h"
#include "Utils.h"
#include "Item.h"

class Enemy
{
public:
	Enemy();
	~Enemy();
	EnemiTypes type;
	sf::Vector2f position;

	void Draw(sf::RenderTexture&, GameTexture&);
	EnemyUpdate Update(float&, PlayerObject&,std::vector<Case*>&);

	std::vector<Item*> DropItems();

	int life = 100;
	int xpBack = 10;
	float damage = 0.5;
private:
	int actualDist = 0;
	float time = 0;
	int size = 50;
	bool isdetected = false;
};

