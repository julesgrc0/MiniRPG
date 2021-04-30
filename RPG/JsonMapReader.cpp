#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "JsonMapReader.h"
#include<thread>
#include "Log.h"

JsonMapReader::JsonMapReader()
{

}

bool JsonMapReader::isset(std::vector<Chunk*>& chunks,int x, int y)
{
    for (Chunk* chunk : chunks)
    {
        if (chunk->position.x == x && chunk->position.y == y)
        {
            return true;
        }
    }
    return false;
}

std::vector<Chunk*> JsonMapReader::getMap()
{
    std::vector<Chunk*> map;
    int i = 0;
    int size = this->map["map"].size();

    LOG() << "[info] Loading " << size << " chunks";

    if (size > 1500)
    {
        return map;
    }

    for (Json::Value jsonChunk : this->map["map"])
    {
        i++;
        if (jsonChunk["chunk"] && jsonChunk["coord"])
        {
            int x = jsonChunk["coord"]["x"].asInt();
            int y = jsonChunk["coord"]["y"].asInt();
            if (!this->isset(map, x, y))
            {
                if (jsonChunk["chunk"].size() == 100)
                {
                    map.push_back(this->LoadJsonChunk(jsonChunk, sf::Vector2f(x, y)));
                }
            }

        }
        LOG() << "[chunk] " << i << "/" << size;
    }

    return map;
}

Chunk* JsonMapReader::LoadJsonChunk(Json::Value& jsonChunk,sf::Vector2f postion)
{
    Chunk* chunk = new Chunk();
    chunk->position = postion;

    for (Json::Value jsonEn : jsonChunk["enemies"])
    {
        if (jsonEn["coord"] && jsonEn["coord"]["x"] && jsonEn["coord"]["y"] && jsonEn["type"])
        {
            for (EnemiTypes type = EnemiTypes::GOBLIN; type != EnemiTypes::ENIMIE_NONE; type = (EnemiTypes)((int)type + 1))
            {
                if (type == jsonEn["type"].asInt())
                {
                    std::pair<sf::Vector2f, EnemiTypes> en = {
                        sf::Vector2f(jsonEn["coord"]["x"].asInt() * 50,jsonEn["coord"]["y"].asInt() * 50),
                        type
                    };
                    chunk->listEnemies.push_back(en);
                }
            }
        }
    }

    for (Json::Value jsonIt : jsonChunk["items"])
    {
        if (jsonIt["coord"] && jsonIt["coord"]["x"] && jsonIt["coord"]["y"] && jsonIt["type"])
        {
            for (ItemTypes type = ItemTypes::SWORD; type != ItemTypes::ITEM_NONE; type = (ItemTypes)((int)type + 1))
            {
                if (type == jsonIt["type"].asInt())
                {
                    std::pair<sf::Vector2f, ItemTypes> it = {
                        sf::Vector2f(jsonIt["coord"]["x"].asInt() * 50,jsonIt["coord"]["y"].asInt() * 50),
                        type
                    };
                    chunk->listItems.push_back(it);
                }
            }
        }
    }

    for (Json::Value jsonBlock : jsonChunk["chunk"])
    {
        Case* block = new Case(50);

        for (CaseTypes type = CaseTypes::GRASS; type != CaseTypes::NONE; type = (CaseTypes)((int)type + 1))
        {
            if ((int)type == jsonBlock.asInt())
            {
                block->case_type = (CaseTypes)type;
            }
        }

        chunk->chunk.push_back(block);
    }

    return chunk;
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
                    cases = this->LoadJsonChunk(jsonChunk, position)->chunk;
                }
            }
        }
    }
    return cases;
}

bool JsonMapReader::loadMap(std::string& path)
{
   FILE* file = fopen(path.c_str(), "r");
    char c;
    std::string content;

    if (!file)
    {
        return false;
    }

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
        this->setupTextures();
        return true;
    }

    return false;
}

void JsonMapReader::setupTextures()
{
    if (this->map["textures"])
    {
        LOG() << "[info] Loading textures";
        if (this->map["textures"]["block"])
        {
            int size = this->map["textures"]["block"].size();
            int i = 0;
            for (Json::Value texture : this->map["textures"]["block"])
            {
                if (texture["id"] && texture["value"])
                {
                    sf::Image image;
                    if (image.loadFromFile((texture["value"].asString())))
                    {
                        image.flipVertically();
                        sf::Texture* block = new sf::Texture();
                        if (block->loadFromImage(image))
                        {
                            std::pair<int, sf::Texture*> p = { texture["id"].asInt(),block };
                            this->block_textures.push_back(p);
                        }
                    }
                }
                i++;
                LOG() << "[texture block] " << i << "/" << size;
            }
        }

        if (this->map["textures"]["enemies"])
        {
            int size = this->map["textures"]["enemies"].size();
            int i = 0;
            for (Json::Value texture : this->map["textures"]["enemies"])
            {
                if (texture["id"] && texture["value"])
                {
                    sf::Image image;
                    if (image.loadFromFile((texture["value"].asString())))
                    {
                        image.flipVertically();
                        sf::Texture* enim = new sf::Texture();
                        if (enim->loadFromImage(image))
                        {
                            std::pair<int, sf::Texture*> p = { texture["id"].asInt(),enim };
                            this->items_textures.push_back(p);
                        }
                    }

                }
                i++;
                LOG() << "[texture enemies] " << i << "/" << size;
            }
        }

        if (this->map["textures"]["player"])
        {
            int size = this->map["textures"]["player"].size();
            int i = 0;
            for (Json::Value texture : this->map["textures"]["player"])
            {
                if (texture["id"] && texture["value"])
                {
                    sf::Image image;
                    if (image.loadFromFile((texture["value"].asString())))
                    {
                        image.flipVertically();
                        sf::Texture* player = new sf::Texture();
                        if (player->loadFromImage(image))
                        {
                            std::pair<int, sf::Texture*> p = { texture["id"].asInt(),player };
                            this->players_textures.push_back(p);
                        }
                    }

                }
                i++;
                LOG() << "[texture player] " << i << "/" << size;
            }
        }

        if (this->map["textures"]["items"])
        {
            int size = this->map["textures"]["items"].size();
            int i = 0;
            for (Json::Value texture : this->map["textures"]["items"])
            {
                if (texture["id"] && texture["value"])
                {
                    sf::Image image;
                    if (image.loadFromFile((texture["value"].asString())))
                    {
                        image.flipVertically();
                        sf::Texture* enim = new sf::Texture();
                        if (enim->loadFromImage(image))
                        {
                            std::pair<int, sf::Texture*> p = { texture["id"].asInt(),enim };
                            this->items_textures.push_back(p);
                        }
                    }

                }
                i++;
                LOG() << "[texture items] " << i << "/" << size;
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