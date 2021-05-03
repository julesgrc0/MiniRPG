#include "Chunk.h"
#include "Utils.h"
#include "Log.h"

Chunk::Chunk()
{
}

void Chunk::Draw(sf::RenderWindow &window, GameTexture& textures)
{
    for (int i = 0; i < 100; i += 10)
    {
        for (int j = 0; j < 10; j++)
        {
            window.draw(this->chunk[static_cast<__int64>(i) + j]->Draw(sf::Vector2f(j * this->size, i * this->size/10), textures));
        }
    }
}

void Chunk::Draw(sf::RenderTexture &texture, GameTexture& textures)
{
    for (int i = 0; i < 100; i += 10)
    {
        for (int j = 0; j < 10; j++)
        {
            texture.draw(this->chunk[i + j]->Draw(sf::Vector2f(j * this->size, i * this->size / 10), textures));
        }
    }

    if (this->listEnemies.size() != 0)
    {
        for (std::pair<sf::Vector2f, EnemiTypes> p : this->listEnemies)
        {
           
            this->DrawEnemies(p, texture, textures);
        }
    }
}

void Chunk::DrawEnemies(std::pair<sf::Vector2f, EnemiTypes> enemy,sf::RenderTexture& texture, GameTexture& textures)
{
    bool hasTexture = false;
    if (textures.enemies_textures.size() != 0)
    {
        for (EnemiTypes type = GOBLIN_S; type != ENIMIES_NONE; type = (EnemiTypes)((int)type + 1))
        {
            if(enemy.second == type)
            {
                sf::Texture* img = new sf::Texture();
                if (getTexture(textures.enemies_textures, type, img))
                {

                    sf::Sprite sprite((*img));
                    sprite.setPosition(enemy.first);
                    texture.draw(sprite);
                    hasTexture =  true;
                }
                break;
            }
        }
    }

    if (!hasTexture)
    {
        sf::CircleShape circ;
        circ.setFillColor(sf::Color::Red);
        circ.setRadius(25.0f);
        circ.setPosition(sf::Vector2f(enemy.first.x , enemy.first.y ));
        texture.draw(circ);
    }
}

ChunkUpdate Chunk::Update(float deltatime, PlayerObject& player)
{
    ChunkUpdate update;

    EnemiTypes goblin[3] = {GOBLIN_S,GOBLIN_R,GOBLIN_L};
    sf::Vector2f p = { round(player.position.x) * 50 ,round(player.position.y) * 50 };

    if (this->listEnemies.size() != 0)
    {
        for (unsigned int i=0;i< this->listEnemies.size();i++)
        {
            if (isTypeEnemy(goblin,this->listEnemies[i].second))
            {
                int dist = 200;
                if (player.isCTRL)
                {
                    dist /= 2;
                }

                if (isPlayerAround(p, this->listEnemies[i].first, dist))
                {
                    if (player.isRight)
                    {
                        this->listEnemies[i].second = GOBLIN_R;
                    }else
                    {
                        this->listEnemies[i].second = GOBLIN_L;
                    }

                    float move = 50 * deltatime;

                    this->listEnemies[i].first = moveEnemyToPlayer(p, this->listEnemies[i].first, move);
                    update.EnemyUpdate = true;
                }else
                {
                    this->listEnemies[i].second = GOBLIN_S;
                } 
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