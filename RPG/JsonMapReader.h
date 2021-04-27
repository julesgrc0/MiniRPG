#pragma once
#include<iostream>
#include<json/json.h>
#include<SFML/Graphics.hpp>
#include "Case.h"

class JsonMapReader
{
public:
	JsonMapReader();
	~JsonMapReader();

	bool loadMap(const char*);
	std::vector<Case*> getChunk(sf::Vector2f);
	sf::Texture getBlockTexture(int index);
	sf::Texture getEnemieTexure(int index);

private:
	void setupTextures();
	Json::Value map;
	std::vector<std::pair<int, sf::Texture>> block_textures;
	std::vector<std::pair<int, sf::Texture>> enemies_textures;
};

