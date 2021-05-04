#include "Item.h"

Item::Item()
{

}

void Item::Draw(sf::RenderTexture& texture, GameTexture& textures)
{
    bool hasTexture = false;
    if (textures.items_textures.size() != 0)
    {
        sf::Texture* img = new sf::Texture();
        if (getTexture(textures.items_textures, this->type, img))
        {
            sf::Sprite sprite((*img));
            sprite.setPosition(this->position);
            sprite.scale(sf::Vector2f(0.5, 0.5));
            texture.draw(sprite);
            hasTexture = true;
        }
    }

    if (!hasTexture)
    {
        sf::CircleShape circ;
        circ.setFillColor(sf::Color::Red);
        circ.setRadius(50.0f/4);
        circ.setPosition(this->position);
        texture.draw(circ);
    }
}

ItemUpdate Item::Update(float& deltatime, PlayerObject& player)
{
	ItemUpdate update;

    sf::Vector2f roundPlayer = sf::Vector2f(round(player.position.x)*50, round(player.position.y)*50);
    sf::Vector2f roundItem = sf::Vector2f(round(this->position.x / 50) * 50, round(this->position.y / 50) * 50);

    if (roundPlayer.x == roundItem.x && roundPlayer.y == roundItem.y)
    {
        update.hasUpdate = true;
        update.type = this->type;
    }

	return update;
}

Item::~Item()
{

}