#pragma once
#include <SFML/Graphics.hpp>
#include "GameTexture.h"

enum CaseTypes
{
	GRASS,
	WATER,
	SAND,
	NONE
};

class Case
{
public:
	Case(int);
	~Case();
	CaseTypes type = NONE;

	void Update(float);
	sf::Sprite Draw(sf::Vector2f, GameTexture& reader);
private:
	void DrawDefault();
	void SetTexture(GameTexture& textures,int id);
	sf::RenderTexture texture;
	float time = 0.0f;
};
