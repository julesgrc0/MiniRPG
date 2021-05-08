#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "JsonMapReader.h"
#include <thread>
#include "Item.h"
#include "Log.h"
#include "Utils.h"

JsonMapReader::JsonMapReader()
{
}

bool JsonMapReader::isset(std::vector<Chunk *> &chunks, int x, int y)
{
    for (Chunk *chunk : chunks)
    {
        if (chunk->position.x == x && chunk->position.y == y)
        {
            return true;
        }
    }
    return false;
}

std::vector<Chunk *> JsonMapReader::getMap()
{
    std::vector<Chunk *> map;
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

Chunk *JsonMapReader::LoadJsonChunk(Json::Value &jsonChunk, sf::Vector2f postion)
{
    Chunk *chunk = new Chunk();
    chunk->position = postion;

    for (Json::Value jsonEn : jsonChunk["enemies"])
    {
        if (jsonEn["coord"] && jsonEn["coord"]["x"] && jsonEn["coord"]["y"] && jsonEn["type"])
        {
            for (EnemiTypes type = EnemiTypes::GOBLIN_S; type != EnemiTypes::ENIMIES_NONE; type = (EnemiTypes)((int)type + 1))
            {
                if (type == jsonEn["type"].asInt())
                {
                    Enemy *enemy = new Enemy();
                    enemy->type = type;
                    enemy->position = sf::Vector2f(jsonEn["coord"]["x"].asInt() * 50, jsonEn["coord"]["y"].asInt() * 50);
                    chunk->listEnemies.push_back(enemy);
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
                    Item *item = new Item();
                    item->position = sf::Vector2f(jsonIt["coord"]["x"].asInt() * 50, jsonIt["coord"]["y"].asInt() * 50);
                    item->type = type;
                    chunk->listItems.push_back(item);
                }
            }
        }
    }

    for (Json::Value jsonBlock : jsonChunk["chunk"])
    {
        Case *block = new Case(50);

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

std::vector<Case *> JsonMapReader::getChunk(sf::Vector2f position)
{
    std::vector<Case *> cases;
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

bool JsonMapReader::loadMap(std::string path)
{
    FILE *file = fopen(path.c_str(), "r");
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
        std::thread([&] {
            setupTextures();
            isTexturesLoad = true;
        }).detach();
        return true;
    }

    return false;
}

void JsonMapReader::setupTexture(Json::Value texture, std::string type, std::vector<std::pair<int, sf::Texture *>> &listTextures)
{
    if (this->map["textures"][type])
    {
        int size = this->map["textures"][type].size();
        int i = 0;
        for (Json::Value texture : this->map["textures"][type])
        {
            if (texture["id"] && texture["value"])
            {
                sf::Image image;
                if (image.loadFromFile((texture["value"].asString())))
                {
                    if (type != "player" && type != "enemies" && type != "items")
                    {
                        image.flipVertically();
                    }

                    sf::Texture *block = new sf::Texture();

                    if (image.getSize().x > 50 || image.getSize().y > 50)
                    {
                        sf::Image newImage = sf::Image();
                        newImage.create(50, 50);
                        resizeImage(image, newImage);

                        if (block->loadFromImage(newImage))
                        {
                            std::pair<int, sf::Texture *> p = {texture["id"].asInt(), block};
                            listTextures.push_back(p);
                        }
                    }
                    else
                    {
                        if (block->loadFromImage(image))
                        {
                            std::pair<int, sf::Texture *> p = {texture["id"].asInt(), block};
                            listTextures.push_back(p);
                        }
                    }
                }
            }

            i++;
            LOG() << "[texture " << type << "] " << i << "/" << size;
        }
    }
}

void JsonMapReader::setupTextures()
{
    if (this->map["textures"])
    {
        LOG() << "[info] Loading textures";

        this->setupTexture(this->map["textures"], "block", this->block_textures);
        this->setupTexture(this->map["textures"], "enemies", this->enemies_textures);
        this->setupTexture(this->map["textures"], "player", this->players_textures);
        this->setupTexture(this->map["textures"], "items", this->items_textures);
    }

    if (this->map["sounds"])
    {
        LOG() << "[info] Loading sounds";
        this->setupSounds(this->map["sounds"]);
    }
}

void JsonMapReader::setupSounds(Json::Value audio)
{
    int size = audio.size();
    int i = 0;
    for (Json::Value sound : audio)
    {
        if (sound["id"] && sound["value"])
        {
            sf::SoundBuffer buff;
            if (buff.loadFromFile(sound["value"].asString()))
            {
                std::pair<int, sf::SoundBuffer> s = {sound["id"].asInt(), buff};
                this->sounds.push_back(s);
            }
        }
        i++;
        LOG() << "[sound] " << i << "/" << size;
    }
}

JsonMapReader::~JsonMapReader()
{
    this->map.clear();
    this->block_textures.clear();
    this->enemies_textures.clear();
    this->items_textures.clear();
    this->players_textures.clear();
}