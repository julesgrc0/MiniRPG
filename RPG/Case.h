#pragma once
#include <SFML/Graphics.hpp>

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
	sf::Sprite Draw(sf::Vector2f);
private:
	sf::RenderTexture texture;
	float time = 0.0f;
};
