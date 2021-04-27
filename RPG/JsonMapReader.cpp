#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "JsonMapReader.h"


JsonMapReader::JsonMapReader()
{

}

std::vector<Case*> JsonMapReader::getChunk(sf::Vector2f position)
{
    std::vector<Case*> cases;
    for (Json::Value jsonChunk : this->map["map"])
    {
        if (jsonChunk["chunk"] && jsonChunk["coord"] && jsonChunk["coord"]["x"] && jsonChunk["coord"]["y"])
        {
            if (jsonChunk["coord"]["x"].asInt() == (int)position.x && jsonChunk["coord"]["y"].asInt() == (int)position.y)
            {
                if (jsonChunk["chunk"].size() == 100)
                {
                    for (Json::Value jsonBlock : jsonChunk["chunk"])
                    {
                        Case* block = new Case(50);
                        block->type = GRASS;
                        switch (jsonBlock.asInt())
                        {
                        case 1:
                            block->type = WATER;
                            break;
                        case 2:
                            block->type = SAND;
                            break;
                        }
                        cases.push_back(block);
                    }
                }
            }
        }
    }
    return cases;
}

sf::Texture JsonMapReader::getBlockTexture(int index)
{
    for (std::pair<int, sf::Texture> p : this->block_textures)
    {
        if (p.first == index)
        {
            return p.second;
        }
    }

    sf::Texture t;
    return t;
}

sf::Texture JsonMapReader::getEnemieTexure(int index)
{
    for (std::pair<int, sf::Texture> p : this->enemies_textures)
    {
        if (p.first == index)
        {
            return p.second;
        }
    }

    sf::Texture t;
    return t;
}

bool JsonMapReader::loadMap(const char* path)
{
    FILE* file = fopen(path, "r");
    char c;
    std::string content;

    while ((c = fgetc(file)) != EOF)
    {
        content += c;
    }

    fclose(file);

    Json::Value root;
    std::istringstream sin(content);

    sin >> this->map;
    if (this->map["map"])
    {
        //  this->setupTextures();
        return true;
    }

    return false;
}

void JsonMapReader::setupTextures()
{
    if (this->map["textures"])
    {
        if (this->map["textures"]["block"])
        {
            for (Json::Value texture : this->map["textures"]["block"])
            {
                if (texture["id"] && texture["value"])
                {
                    sf::Texture block;
                    if(block.loadFromFile(texture["value"].asString()))
                    {
                        std::pair<int, sf::Texture> p = { texture["id"].asInt(),block };
                        this->block_textures.push_back(p);
                    }
                }
            }
        }

        if (this->map["textures"]["enemies"])
        {
            for (Json::Value texture : this->map["textures"]["enemies"])
            {
                if (texture["id"] && texture["value"])
                {
                    sf::Texture enim;

                    if (enim.loadFromFile(texture["value"].asString()))
                    {
                        std::pair<int, sf::Texture> p = { texture["id"].asInt(),enim };
                        this->enemies_textures.push_back(p);
                    }
                }
            }
        }
    }
}

JsonMapReader::~JsonMapReader()
{
    this->map.clear();
    this->block_textures.clear();
    this->enemies_textures.clear();
}