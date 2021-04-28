#pragma once
#include<iostream>
#include<json/json.h>
#include<SFML/Graphics.hpp>
#include "Case.h"
#include "Chunk.h"

class JsonMapReader
{
public:
	JsonMapReader();
	~JsonMapReader();

	bool loadMap(std::string&);
	std::vector<Case*> getChunk(sf::Vector2f);
	std::vector<Chunk*> getMap();
	sf::Texture getBlockTexture(int index);
	sf::Texture getEnemieTexure(int index);
private:
	void setupTextures();
	bool isset(std::vector<Chunk*>&,int, int);
	Json::Value map;
	std::vector<std::pair<int, sf::Texture>> block_textures;
	std::vector<std::pair<int, sf::Texture>> enemies_textures;
};

