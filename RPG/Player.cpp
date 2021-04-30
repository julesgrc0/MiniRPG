#include "Player.h"
#include "Log.h"
#include "Utils.h"

Player::Player()
{
}
void Player::Draw(sf::RenderWindow& window, GameTexture& textures)
{
    sf::CircleShape circ;
    circ.setFillColor(sf::Color::Black);
    circ.setRadius(25.0f);
    circ.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50));
    window.draw(circ);
}

void Player::Draw(sf::RenderTexture& texture, GameTexture& textures)
{
    sf::Texture* textureItem = new sf::Texture();
    if (this->getTexture(this->activeState, textures,textureItem))
    {
        sf::Sprite sprite = sf::Sprite(*textureItem);
        sprite.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50));
        texture.draw(sprite);
    }else
    {
        sf::CircleShape circ;
        circ.setFillColor(sf::Color::Black);
        circ.setRadius(25.0f);
        circ.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50));
        texture.draw(circ);
    }
}

bool Player::getTexture(PlayerStates state, GameTexture& textures,sf::Texture*& textureItem)
{
    for (std::pair<int, sf::Texture*> p : textures.players_textures)
    {
        if (p.first == (int)state)
        {
            textureItem = p.second;
            return true;
        }
    }

    return false;
}

void Player::addVisitedChunk(sf::Vector2f chunk)
{
    if (std::find(visitedChunks.begin(), visitedChunks.end(), chunk) == visitedChunks.end())
    {
        this->visitedChunks.push_back(chunk);
    }
}

void Player::goForward()
{
    this->playerPos = this->lastPos;
}

bool Player::isCollisionBlock(CaseTypes& type)
{
    for (CaseTypes Itemtype : this->collisions)
    {
        if (type == Itemtype)
        {
            return true;
        }
    }

    return false;
}

bool Player::KeyBoardUpdate(float deltatime, std::vector<Case*>& chunk)
{
    bool keyboardUpdate = false;

     sf::Vector2f lpos = this->playerPos;

     sf::Vector2f roundPlayer = sf::Vector2f(round(this->playerPos.x), round(this->playerPos.y));
     CaseTypes type = getCaseByPosition(chunk, roundPlayer)->case_type;

     if (type != CaseTypes::NONE)
     {
         switch (type)
         {
         case WATER:
             this->activeState = S_WATER;
             break;
         default:
         {
             if (isDirectionR)
             {
                 this->activeState = STATIC_R;
             }
             else
             {
                 this->activeState = STATIC_L;
             }
         }
         break;
         }
     }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        playerSeep = 2;
    }
    else
    {
        this->playerSeep = this->Max_player_speed;
    }

    if (this->activeState != S_WATER)
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            this->activeState = S_ATTACK;
            playerSeep = this->Max_player_speed - 2;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            this->activeState = S_DEFEND;
            playerSeep = 2;
        }
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
        this->isDirectionR = true;
        this->playerPos.x -= playerSeep * deltatime;
        keyboardUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->isDirectionR = false;
        this->playerPos.x += playerSeep * deltatime;
        keyboardUpdate = true;
    }

    if ((this->activeState == STATIC_L || this->activeState == STATIC_R))
    {
        if (isDirectionR)
        {
            this->activeState = STATIC_R;
        }
        else
        {
            this->activeState = STATIC_L;
        }
    }


    if (keyboardUpdate)
    {
        this->lastPos = lpos;
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

std::vector<Case*> Player::getChunk(std::vector<Chunk*>& chunks,sf::Vector2f position)
{
    for (Chunk* chunk : chunks)
    {
        if (chunk->position == position)
        {
            return chunk->chunk;
        }
    }
    std::vector<Case*> cases;
    return cases;
}

bool Player::MapUpdate(float deltatime, std::vector<Chunk*>& chunks, Chunk*& activeChunk)
{
    this->isNew = false;
    bool chunkChange = false;

    sf::Vector2f roundPlayer = sf::Vector2f((this->playerPos.x * 50 / 50), (this->playerPos.y * 50 / 50));
    sf::Vector2f next;

    float max_out_detection = 9.5;
    float min_out_detection = -0.5;

    if (roundPlayer.x > max_out_detection)
    {
        next = activeChunk->position;
        next.x++;
        std::vector<Case*> c = this->getChunk(chunks,next);
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
    else if (roundPlayer.x < min_out_detection)
    {
        next = activeChunk->position;
        next.x--;
        std::vector<Case*> c = this->getChunk(chunks, next);
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
    
    if (roundPlayer.y > max_out_detection)
    {
        next = activeChunk->position;
        next.y++;
        std::vector<Case*> c = this->getChunk(chunks, next);
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
    else if (roundPlayer.y < min_out_detection)
    {
        next = activeChunk->position;
        next.y--;
        std::vector<Case*> c = this->getChunk(chunks, next);
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
        LOG() << "[chunk change] (" << activeChunk->position.x << ";" << activeChunk->position.y << ")";
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
    this->visitedChunks.clear();
}