#include "HUD.h"
#include "Log.h"

HUD::HUD(sf::Font &f)
{
	this->font = f;
}

void HUD::Draw(sf::RenderWindow &window, sf::Sprite &game, GameTexture &textures)
{
	int topMargin = 1;
	game.setPosition(sf::Vector2f(500 + topMargin, topMargin));
	game.setScale(sf::Vector2f(0.2, 0.2));
	window.draw(game);

	sf::RectangleShape outlineGame;
	outlineGame.setSize(sf::Vector2f(100, 100));
	outlineGame.setPosition(sf::Vector2f(500 + topMargin, topMargin));
	outlineGame.setFillColor(sf::Color::Transparent);
	outlineGame.setOutlineColor(sf::Color::Color(196, 196, 196, 255));
	outlineGame.setOutlineThickness(1.0f);
	window.draw(outlineGame);

	sf::RectangleShape inventoryRect;
	inventoryRect.setSize(sf::Vector2f(300, 100 + topMargin * 2));
	inventoryRect.setPosition(sf::Vector2f(500, 100 + topMargin * 2));
	inventoryRect.setFillColor(sf::Color::Color(49, 49, 49, 255));
	window.draw(inventoryRect);

	sf::RectangleShape itemRect;
	itemRect.setSize(sf::Vector2f(75, 50));
	itemRect.setOutlineColor(sf::Color::Color(49, 49, 49, 255));
	itemRect.setOutlineThickness(2.0f);

	sf::Sprite spriteItem;
	sf::Texture *textureItem = new sf::Texture();
	bool hasTexture = false;

	for (size_t i = 0; i < 8; i++)
	{
		hasTexture = false;

		if (getTexture(textures.items_textures, (int)this->active.activeBar[i], textureItem))
		{
			hasTexture = true;
			spriteItem.setTexture((*textureItem));
		}

		if (i == this->active.activeBarIndex)
		{
			itemRect.setFillColor(sf::Color::Color(111, 111, 111, 255));
		}
		else
		{
			itemRect.setFillColor(sf::Color::Color(77, 77, 77, 255));
		}

		if (i > 3)
		{
			itemRect.setPosition(sf::Vector2f(500 + (i - 4) * 75 + topMargin * 2, 100 + topMargin * 6 + 50));
			window.draw(itemRect);
			if (hasTexture)
			{
				spriteItem.setPosition(sf::Vector2f(500 + (i - 4) * 75 + 25 / 2, 100 + topMargin * 6 + 50));
				window.draw(spriteItem);
			}
		}
		else
		{
			itemRect.setPosition(sf::Vector2f(500 + i * 75 + topMargin * 2, 100 + topMargin * 3));
			window.draw(itemRect);
			if (hasTexture)
			{
				spriteItem.setPosition(sf::Vector2f(500 + i * 75 + 25 / 2, 100 + topMargin * 3));
				window.draw(spriteItem);
			}
		}
	}

	sf::RectangleShape statsRect;
	statsRect.setSize(sf::Vector2f(300, 500 - (200 + topMargin * 4)));
	statsRect.setPosition(sf::Vector2f(500, 200 + topMargin * 4));
	statsRect.setFillColor(sf::Color::Color(49, 49, 49, 255));
	window.draw(statsRect);

	sf::Vector2f textPos = sf::Vector2f(510, 200 + topMargin * 4 + 10);

	sf::Text t;
	t.setFont(this->font);
	t.setCharacterSize(14);

	t.setPosition(textPos);
	t.setString("frames time: " + std::to_string(this->active.deltatime) + "ms");
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("kills: " + std::to_string(this->active.playerKill));
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("life: " + std::to_string(this->active.playerLife) + "%");
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("experience: " + std::to_string(this->active.playerXP));
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("x: " + std::to_string(this->active.playerPos.x) + " y:" + std::to_string(this->active.playerPos.y));
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("inventory: " + std::to_string(this->active.inventoryItemsCount));
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("chunk enemies: " + std::to_string(this->active.enemiesCount));
	window.draw(t);

	textPos.y += 20;
	t.setPosition(textPos);
	t.setString("night: " + std::to_string(this->active.night) + "%");
	window.draw(t);
}

void HUD::Update(HUDupdateObject &update)
{
	this->active = update;
}

HUD::~HUD()
{
}