#include "MapReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"

MapReader::MapReader()
{
}

std::vector<Chunk *> MapReader::getFullMap(const char *filepath)
{
    std::ifstream file(filepath);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            Chunk *chunk = new Chunk();
            std::vector<std::string> numbers = split(line, ' ');

            if (numbers.size() >= 102)
            {
                float x = std::atoi(numbers[0].c_str()), y = std::atoi(numbers[1].c_str());
                chunk->position = sf::Vector2f(x, y);

                for (int i = 2; i < 102; i++)
                {
                    Case *item = new Case();
                    CaseTypes type;
                    switch (std::atoi(numbers[i].c_str()))
                    {
                    case 0:
                        type = NONE;
                        break;
                    case 1:
                        type = GRASS;
                        break;
                    case 2:
                        type = WATER;
                        break;
                    case 3:
                        type = SAND;
                        break;
                    }
                    item->type = type;

                    chunk->chunk.push_back(item);
                }
                this->map_chunks.push_back(chunk);
            }
            std::vector<std::string>().swap(numbers);
        }
        file.close();
    }
    return this->map_chunks;
}

std::vector<Case *> MapReader::getMapChunk(const char *filepath, sf::Vector2f pos)
{
    std::vector<Case *> res;
    std::ifstream file(filepath);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> numbers = split(line, ' ');

            if (numbers.size() >= 102)
            {
                float x = std::atoi(numbers[0].c_str()), y = std::atoi(numbers[1].c_str());

                if (x == pos.x && y == pos.y)
                {
                    for (int i = 2; i < 102; i++)
                    {
                        Case *item = new Case();
                        CaseTypes type;
                        switch (std::atoi(numbers[i].c_str()))
                        {
                        case 0:
                            type = NONE;
                            break;
                        case 1:
                            type = GRASS;
                            break;
                        case 2:
                            type = WATER;
                            break;
                        case 3:
                            type = SAND;
                            break;
                        }

                        item->type = type;
                        res.push_back(item);
                    }
                }
            }
            std::vector<std::string>().swap(numbers);
        }
        file.close();
    }
    return res;
}

MapReader::~MapReader()
{
    std::vector<Chunk *>().swap(this->map_chunks);
}