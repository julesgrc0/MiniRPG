#include "GameTexture.h"


GameTexture::GameTexture()
{

}

GameTexture::~GameTexture()
{
	this->block_textures.clear();
	this->enemies_textures.clear();
	this->items_textures.clear();
	this->players_textures.clear();
}