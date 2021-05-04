#include "Player.h"
#include "Log.h"
#include "Utils.h"

Player::Player()
{
    for (size_t i = 0; i < 8; i++)
    {
        this->activeBar[i] = WOOD;// ITEM_NONE;
    }
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

    sf::RectangleShape lifeRect;
    lifeRect.setSize(sf::Vector2f(50, 8));
    lifeRect.setPosition(sf::Vector2f(this->playerPos.x*50, this->playerPos.y * 50 -  10));
    lifeRect.setOutlineColor(sf::Color::Black);
    lifeRect.setOutlineThickness(1.0f);
    lifeRect.setFillColor(sf::Color::Transparent);
    texture.draw(lifeRect);

    sf::RectangleShape lifeFill;
    lifeFill.setSize(sf::Vector2f(((50 * this->life) / 100), 8));
    lifeFill.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50 - 10));
    lifeFill.setFillColor(sf::Color::Green);
    texture.draw(lifeFill);
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

PlayerUpdate Player::KeyBoardUpdate(float deltatime, Chunk*& chunk)
{
    PlayerUpdate update;

     sf::Vector2f lpos = this->playerPos;

     sf::Vector2f roundPlayer = sf::Vector2f(round(this->playerPos.x), round(this->playerPos.y));
     CaseTypes type = getCaseByPosition(chunk->chunk, roundPlayer)->case_type;


     sf::Vector2f pO = sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50);
     std::vector<int> eraselist;

     for (unsigned int i=0;i < chunk->listEnemies.size();i++)
     {
         if (isPlayerAround(pO, chunk->listEnemies[i]->position, 50) && (this->activeState != PlayerStates::S_DEFEND_L && this->activeState != PlayerStates::S_DEFEND_R))
         {
             if (this->activeState == PlayerStates::S_ATTACK_L || this->activeState == PlayerStates::S_ATTACK_R)
             {
                 enemiesDmgTime += deltatime;
                 if (damageTime > this->damage)
                 {
                     enemiesDmgTime = 0.0f;
                     chunk->listEnemies[i]->life -= 1;
                     if (chunk->listEnemies[i]->life < 0)
                     {
                         this->kill++;
                         std::vector<Item*> items = chunk->listEnemies[i]->DropItems();
                         chunk->listItems.insert(chunk->listItems.end(), items.begin(), items.end());
                         eraselist.push_back(i);
                     }
                 }
             }

             damageTime += deltatime;
             if (damageTime > chunk->listEnemies[i]->damage)
             {
                 damageTime = 0.0f;
                 this->life -= 1;
             }
             update.hasUpdate = true;
         }
     }

     if (eraselist.size() != 0)
     {
         update.hasChunkUpdate = true;
         
         for (size_t i = 0; i < eraselist.size(); i++)
         {
             chunk->listEnemies.erase(chunk->listEnemies.begin() + eraselist[i] - i);
         }

         update.chunkUpdate = chunk;
         eraselist.clear();
     }

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

     if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
     {
         rCTRLpressTime += deltatime;
         if (rCTRLpressTime >= 0.15)
         {
             this->HUDbarIndex++;
             if (this->HUDbarIndex > 7)
             {
                 this->HUDbarIndex = 0;
             }
             update.hasUpdate = true;
             rCTRLpressTime = 0.0f;
         }
     }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        playerSeep = 2;
        this->isCTRL = true;
    }
    else
    {
        this->isCTRL = false;
        this->playerSeep = this->Max_player_speed;
    }

    if (this->activeState != S_WATER)
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            if (isDirectionR)
            {
                this->activeState = S_ATTACK_L;
            }else
            {
                this->activeState = S_ATTACK_R;
            }
           
            playerSeep = this->Max_player_speed - 2;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (isDirectionR)
            {
                this->activeState = S_DEFEND_L;
            }
            else
            {
                this->activeState = S_DEFEND_R;
            }
            playerSeep = 2;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        this->playerPos.y -= playerSeep * deltatime;
        update.hasUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        this->playerPos.y += playerSeep * deltatime;
        update.hasUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->isDirectionR = true;
        this->playerPos.x -= playerSeep * deltatime;
        update.hasUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->isDirectionR = false;
        this->playerPos.x += playerSeep * deltatime;
        update.hasUpdate = true;
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


    if (update.hasUpdate)
    {
        this->lastPos = lpos;
    }
   
    return update;
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

Chunk* Player::getChunk(std::vector<Chunk*>& chunks,sf::Vector2f position)
{
    Chunk* chunk = new Chunk();
    for (Chunk* c : chunks)
    {
        if (c->position == position)
        {
            chunk->chunk = c->chunk;
            chunk->listEnemies = c->listEnemies;
            chunk->listItems = c->listItems;
            break;
        }
    }
    return chunk;
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
        Chunk* c = this->getChunk(chunks, next);
        if (c->chunk.size() > 0)
        {
            this->playerPos.x -= 9.5;
            this->playerPos.x += playerSeep * deltatime;

            activeChunk->chunk = c->chunk;
            activeChunk->listEnemies = c->listEnemies;
            activeChunk->listItems = c->listItems;
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
        Chunk* c = this->getChunk(chunks, next);
        if (c->chunk.size() > 0)
        {
            this->playerPos.x += 9.5;
            this->playerPos.x -= playerSeep * deltatime;

            activeChunk->chunk = c->chunk;
            activeChunk->listEnemies = c->listEnemies;
            activeChunk->listItems = c->listItems;
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
        Chunk* c = this->getChunk(chunks, next);
        if (c->chunk.size() > 0)
        {
            this->playerPos.y -= 9.5;
            this->playerPos.y += playerSeep * deltatime;

            activeChunk->chunk = c->chunk;
            activeChunk->listEnemies = c->listEnemies;
            activeChunk->listItems = c->listItems;
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
        Chunk* c = this->getChunk(chunks, next);
        if (c->chunk.size() > 0)
        {
            this->playerPos.y += 9.5;
            this->playerPos.y -= playerSeep * deltatime;

            activeChunk->chunk = c->chunk;
            activeChunk->listEnemies = c->listEnemies;
            activeChunk->listItems = c->listItems;
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