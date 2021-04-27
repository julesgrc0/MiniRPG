#include "Chunk.h"

Chunk::Chunk()
{
}

void Chunk::Draw(sf::RenderWindow &window)
{
    for (int i = 0; i < 100; i += 10)
    {
        for (int j = 0; j < 10; j++)
        {
            window.draw(this->chunk[static_cast<__int64>(i) + j]->Draw(sf::Vector2f(j * this->size, i * this->size/10)));
        }
    }
}

void Chunk::Draw(sf::RenderTexture &texture)
{
    for (int i = 0; i < 100; i += 10)
    {
        for (int j = 0; j < 10; j++)
        {
            texture.draw(this->chunk[static_cast<__int64>(i) + j]->Draw(sf::Vector2f(j * this->size, i * this->size / 10)));
        }
    }
}

Chunk::~Chunk()
{
}