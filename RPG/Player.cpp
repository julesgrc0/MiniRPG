#include "Player.h"
#include "Consts.h"

Player::Player()
{
}
void Player::Draw(sf::RenderWindow &window)
{
    sf::CircleShape circ;
    circ.setFillColor(sf::Color::Black);
    circ.setRadius(25.0f);
    circ.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50));
    window.draw(circ);
}

void Player::Draw(sf::RenderTexture &texture)
{
    sf::CircleShape circ;
    circ.setFillColor(sf::Color::Black);
    circ.setRadius(25.0f);
    circ.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50));
    texture.draw(circ);
}

void Player::addVisitedChunk(sf::Vector2f chunk)
{
    if (std::find(visitedChunks.begin(), visitedChunks.end(), chunk) == visitedChunks.end())
    {
        this->visitedChunks.push_back(chunk);
    }
}

bool Player::KeyBoardUpdate(float deltatime)
{
    bool keyboardUpdate = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        playerSeep = 2;
    }
    else
    {
        this->playerSeep = this->Max_player_speed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        this->playerPos.y -= playerSeep * deltatime;
        keyboardUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        this->playerPos.y += playerSeep * deltatime;
        keyboardUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->playerPos.x -= playerSeep * deltatime;
        keyboardUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->playerPos.x += playerSeep * deltatime;
        keyboardUpdate = true;
    }

    return keyboardUpdate;
}

bool Player::isNewChunk()
{
    if (this->isNew)
    {
        this->isNew = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool Player::MapUpdate(float deltatime, MapReader &reader, Chunk *&activeChunk)
{
    this->isNew = false;
    bool chunkChange = false;

    sf::Vector2f roundPlayer = sf::Vector2f((this->playerPos.x * 50 / 50), (this->playerPos.y * 50 / 50));
    sf::Vector2f next;

    if (roundPlayer.x > 9.9)
    {
        next = activeChunk->position;
        next.x++;
        std::vector<Case *> c = reader.getMapChunk(MAP_PATH, next);
        if (c.size() > 0)
        {
            this->playerPos.x -= 9.5;
            this->playerPos.x += playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.x -= playerSeep * deltatime;
        }
    }
    else if (roundPlayer.x < -0.5)
    {
        next = activeChunk->position;
        next.x--;
        std::vector<Case *> c = reader.getMapChunk(MAP_PATH, next);
        if (c.size() > 0)
        {
            this->playerPos.x += 9.5;
            this->playerPos.x -= playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.x += playerSeep * deltatime;
        }
    }
    else if (roundPlayer.y > 9.5)
    {
        next = activeChunk->position;
        next.y++;
        std::vector<Case *> c = reader.getMapChunk(MAP_PATH, next);
        if (c.size() > 0)
        {
            this->playerPos.y -= 9.5;
            this->playerPos.y += playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.y -= playerSeep * deltatime;
        }
    }
    else if (roundPlayer.y < -0.5)
    {
        next = activeChunk->position;
        next.y--;
        std::vector<Case *> c = reader.getMapChunk(MAP_PATH, next);
        if (c.size() > 0)
        {
            this->playerPos.y += 9.5;
            this->playerPos.y -= playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.y += playerSeep * deltatime;
        }
    }

    if (chunkChange)
    {
        if (std::find(visitedChunks.begin(), visitedChunks.end(), activeChunk->position) == visitedChunks.end())
        {
            this->isNew = true;
            this->visitedChunks.push_back(activeChunk->position);
        }
    }

    return chunkChange;
}


bool Player::MapUpdate(float deltatime, JsonMapReader& reader, Chunk*& activeChunk)
{
    this->isNew = false;
    bool chunkChange = false;

    sf::Vector2f roundPlayer = sf::Vector2f((this->playerPos.x * 50 / 50), (this->playerPos.y * 50 / 50));
    sf::Vector2f next;

    if (roundPlayer.x > 9.9)
    {
        next = activeChunk->position;
        next.x++;
        std::vector<Case*> c = reader.getChunk(next);
        if (c.size() > 0)
        {
            this->playerPos.x -= 9.5;
            this->playerPos.x += playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.x -= playerSeep * deltatime;
        }
    }
    else if (roundPlayer.x < -0.5)
    {
        next = activeChunk->position;
        next.x--;
        std::vector<Case*> c = reader.getChunk(next);
        if (c.size() > 0)
        {
            this->playerPos.x += 9.5;
            this->playerPos.x -= playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.x += playerSeep * deltatime;
        }
    }
    else if (roundPlayer.y > 9.5)
    {
        next = activeChunk->position;
        next.y++;
        std::vector<Case*> c = reader.getChunk(next);
        if (c.size() > 0)
        {
            this->playerPos.y -= 9.5;
            this->playerPos.y += playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.y -= playerSeep * deltatime;
        }
    }
    else if (roundPlayer.y < -0.5)
    {
        next = activeChunk->position;
        next.y--;
        std::vector<Case*> c = reader.getChunk(next);
        if (c.size() > 0)
        {
            this->playerPos.y += 9.5;
            this->playerPos.y -= playerSeep * deltatime;

            activeChunk->chunk = c;
            activeChunk->position = next;
            chunkChange = true;
        }
        else
        {
            this->playerPos.y += playerSeep * deltatime;
        }
    }

    if (chunkChange)
    {
        if (std::find(visitedChunks.begin(), visitedChunks.end(), activeChunk->position) == visitedChunks.end())
        {
            this->isNew = true;
            this->visitedChunks.push_back(activeChunk->position);
        }
    }

    return chunkChange;
}

Player::~Player()
{
}