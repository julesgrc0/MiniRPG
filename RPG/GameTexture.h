#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>

class GameTexture
{
public:
	GameTexture();
	~GameTexture();
	std::vector<std::pair<int, sf::Texture*>> block_textures;
	std::vector < std::pair<int, sf::Texture*>> enemies_textures;
};

