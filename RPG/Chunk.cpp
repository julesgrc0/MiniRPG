#include "Chunk.h"
#include "Utils.h"
#include "Log.h"

Chunk::Chunk()
{
}

void Chunk::Draw(sf::RenderWindow& window, GameTexture& textures)
{
    for (int i = 0; i < 100; i += 10)
    {
        for (int j = 0; j < 10; j++)
        {
            window.draw(this->chunk[static_cast<__int64>(i) + j]->Draw(sf::Vector2f(j * this->size, i * this->size / 10), textures));
        }
    }
}

Chunk& Chunk::operator = (const Chunk*& chunk)
{
    this->size = chunk->size;
    this->listEnemies = chunk->listEnemies;
    this->listItems = chunk->listItems;
    this->chunk = chunk->chunk;
    this->isNight = chunk->isNight;
    this->position = chunk->position;

    return *this;
}

void Chunk::Draw(sf::RenderTexture& texture, GameTexture& textures)
{
    for (int i = 0; i < 100; i += 10)
    {
        for (int j = 0; j < 10; j++)
        {
            texture.draw(this->chunk[i + j]->Draw(sf::Vector2f(j * this->size, i * this->size / 10), textures));
        }
    }

    if (this->listEnemies.size() != 0 && this->isNight)
    {
        for (Enemy* p : this->listEnemies)
        {
            p->Draw(texture, textures);
        }
    }

    if (this->listItems.size() != 0)
    {
        for (Item* it : this->listItems)
        {
            it->Draw(texture, textures);
        }
    }
}

ChunkUpdate Chunk::Update(float deltatime, PlayerObject& player,std::vector<Case*>& cases)
{
    this->isNight = player.isNight;
    ChunkUpdate update;

    if (this->listEnemies.size() != 0 && this->isNight)
    {
        for (Enemy* p : this->listEnemies)
        {
            if(p->Update(deltatime, player, cases).hasUpdate)
            {
                update.EnemyUpdate = true;
            }
        }
    }

    if (this->listItems.size() != 0)
    {
        for (Item* it : this->listItems)
        {
            if (it->Update(deltatime, player).hasUpdate)
            {
                update.ItemUpdate = true;
                std::pair<ItemTypes, sf::Vector2f> col = { it->type,it->position };
                update.ItemsCollect.push_back(col);
            }
        }
    }

    return update;
}



Chunk::~Chunk()
{
    this->chunk.clear();
    this->listEnemies.clear();
    this->listItems.clear();
}