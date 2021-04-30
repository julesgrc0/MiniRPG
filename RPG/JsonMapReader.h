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

	bool loadMap(std::string);
	std::vector<Case*> getChunk(sf::Vector2f);
	std::vector<Chunk*> getMap();

	std::vector<std::pair<int, sf::Texture*>> block_textures;
	std::vector<std::pair<int, sf::Texture*>> enemies_textures;
	std::vector<std::pair<int, sf::Texture*>> items_textures;
	std::vector<std::pair<int, sf::Texture*>> players_textures;

	bool isTexturesLoad = false;
private:
	Chunk* LoadJsonChunk(Json::Value&, sf::Vector2f);
	void setupTextures();
	void setupTexture(Json::Value texture, std::string type, std::vector<std::pair<int, sf::Texture*>>&);
	bool isset(std::vector<Chunk*>&,int, int);
	
	Json::Value map;
};

