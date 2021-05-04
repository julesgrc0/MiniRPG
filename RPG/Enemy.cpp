#include "Enemy.h"
#include "Utils.h"
#include"Log.h"

void Enemy::Draw(sf::RenderTexture& texture, GameTexture& textures)
{
        sf::RectangleShape line;
        int detection = (this->actualDist + this->size * 2);
        line.setSize(sf::Vector2f(detection, detection));


        if (!this->isdetected)
        {
            line.setFillColor(sf::Color::Color(70, 70, 70, 70));
            line.setOutlineColor(sf::Color::Black);
        }
        else
        {
            line.setFillColor(sf::Color::Color(255, 0, 0, 70));
            line.setOutlineColor(sf::Color::Red);
        }

        line.setOutlineThickness(1.0f);
        line.setOrigin(sf::Vector2f((detection / 2) - this->size / 2, (detection / 2) - this->size / 2));
        line.setPosition(this->position);
        texture.draw(line);

        bool hasTexture = false;
        if (textures.enemies_textures.size() != 0)
        {
            sf::Texture* img = new sf::Texture();
            if (getTexture(textures.enemies_textures, this->type, img))
            {
                sf::Sprite sprite((*img));
                sprite.setPosition(this->position);
                texture.draw(sprite);
                hasTexture = true;
            }
        }
        
        sf::RectangleShape lifeRect;
        lifeRect.setSize(sf::Vector2f(this->size, 8));
        lifeRect.setPosition(sf::Vector2f(this->position.x, this->position.y - this->size/2));
        lifeRect.setOutlineColor(sf::Color::Black);
        lifeRect.setOutlineThickness(1.0f);
        lifeRect.setFillColor(sf::Color::Transparent);
        texture.draw(lifeRect);

        sf::RectangleShape lifeFill;
        lifeFill.setSize(sf::Vector2f(((this->size * life)/100), 8));
        lifeFill.setPosition(sf::Vector2f(this->position.x, this->position.y - this->size / 2));
        lifeFill.setFillColor(sf::Color::Red);
        texture.draw(lifeFill);


        if (!hasTexture)
        {
            sf::CircleShape circ;
            circ.setFillColor(sf::Color::Red);
            circ.setRadius(25.0f);
            circ.setPosition(this->position);
            texture.draw(circ);
        }
}

EnemyUpdate Enemy::Update(float& deltatime, PlayerObject& player, std::vector<Case*>& cases)
{
    EnemyUpdate update;
    EnemiTypes goblin[4] = { GOBLIN_WATER,GOBLIN_S,GOBLIN_R,GOBLIN_L, };

    sf::Vector2f p = { round(player.position.x) * 50 ,(player.position.y) * 50 };
    time += deltatime;
        if (isTypeEnemy(goblin, this->type, 4))
        {
            this->actualDist = 200;
            if (player.isCTRL)
            {
                this->actualDist /= 2;
            }

            if (isPlayerAround(p, this->position, this->actualDist))
            {
                if (player.isRight)
                {
                    this->type = GOBLIN_R;
                }
                else
                {
                    this->type = GOBLIN_L;
                }

                float move = 100 * deltatime;
                sf::Vector2f newPos = movePlayerToEnemyDist(p, this->position, move,this->size);
                sf::Vector2f roundPlayer = sf::Vector2f(round(player.position.x), round(player.position.y));

                if (getCaseByPosition(cases, roundPlayer)->case_type != CaseTypes::WATER)
                {
                    if (getCaseByPosition(cases, sf::Vector2f(round(newPos.x / this->size), round(newPos.y / this->size)))->case_type == CaseTypes::WATER)
                    {
                        this->type = GOBLIN_WATER;
                    }

                    this->position = newPos;
                    this->isdetected = true;
                }
                else
                {
                    this->isdetected = false;
                }

                update.hasUpdate = true;
            }
            else
            {
                this->isdetected = false;
                this->type = GOBLIN_S;
            }
    }

    return update;
}

std::vector<Item*> Enemy::DropItems()
{
    std::vector<Item*> items;
    Item* item = new Item();
    item->position = this->position;
    item->type = ItemTypes::WOOD;
    items.push_back(item);
    return items;
}

Enemy::Enemy(){}
Enemy::~Enemy(){}

