#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "MainGame.h"
#include "MapReader.h"
#include "Consts.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

bool MainGame::Init(const char *path)
{
    return true;
}

void MainGame::Run()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "", sf::Style::Close);
    //sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "",sf::Style::Fullscreen | sf::Style::Close);

    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(0);

    //window.setMouseCursorVisible(false);

    sf::Clock clock;
    float deltatime = 0;
    this->player.playerPos = sf::Vector2f(4, 4);

    MapReader reader = MapReader();
    Chunk *activeChunk = new Chunk();

    activeChunk->position = sf::Vector2f(0, 0);
    activeChunk->chunk = reader.getMapChunk(MAP_PATH, activeChunk->position);
    this->player.addVisitedChunk(activeChunk->position);

    bool mouseUpdate = true;
    bool keyboardUpdate = true;

    bool chunkChange = false;
    bool endAnimation = false;
    const float chunkAnimationDuration = 20.0f;
    float chunkChangeAnimation = chunkAnimationDuration;

    time_t start = time(0);
    time_t current = time(0);
    int frames = 0;

    sf::RenderTexture gameTexture;
    gameTexture.create(500, 500);

    while (window.isOpen())
    {
        deltatime = clock.restart().asSeconds();

        frames++;
        current = time(0);
        if (current - start >= (time_t)1)
        {
            window.setTitle(std::to_string(frames) + " (" + std::to_string((int)activeChunk->position.x) + ";" + std::to_string((int)activeChunk->position.y) + ")");
            frames = 0;
            start = time(0);
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        keyboardUpdate = this->player.KeyBoardUpdate(deltatime);
        if (keyboardUpdate)
        {
            chunkChange = this->player.MapUpdate(deltatime, reader, activeChunk);
        }

        mouseUpdate = this->mouse.Update(window);

        for (int i = 0; i < activeChunk->chunk.size(); i++)
        {
            activeChunk->chunk[i]->Update(deltatime);
        }

        if (chunkChange || chunkChangeAnimation < chunkAnimationDuration)
        {
            if (chunkChange)
            {
                if (this->player.isNewChunk())
                {
                    chunkChangeAnimation = 0.0f;
                    endAnimation = false;
                }

                chunkChange = false;
            }
            else
            {
                chunkChangeAnimation += deltatime * 10;
                if (chunkChangeAnimation > chunkAnimationDuration)
                {
                    endAnimation = true;
                }
            }

            if (!endAnimation)
            {

                gameTexture.clear();

                activeChunk->Draw(gameTexture);
                this->player.Draw(gameTexture);
                this->mouse.Draw(gameTexture);
                sf::RectangleShape blur;
                int gradien = 255 - (int)((255 * chunkChangeAnimation * (100 / chunkAnimationDuration)) / 100);
                blur.setFillColor(sf::Color(0, 0, 0, gradien));
                blur.setPosition(sf::Vector2f(0, 0));
                blur.setSize(sf::Vector2f(window.getSize()));
                gameTexture.draw(blur);

                gameTexture.display();

                window.clear();
                sf::Sprite gameSprite(gameTexture.getTexture());
                gameSprite.setPosition(sf::Vector2f(10, 10));
                window.draw(gameSprite);
                window.display();
            }
        }
        else
        {
            if (keyboardUpdate || mouseUpdate || endAnimation)
            {
                endAnimation = false;
                keyboardUpdate = false;
                mouseUpdate = false;

                gameTexture.clear();

                activeChunk->Draw(gameTexture);
                this->player.Draw(gameTexture);
                this->mouse.Draw(gameTexture);

                gameTexture.display();

                window.clear();
                sf::Sprite gameSprite(gameTexture.getTexture());
                gameSprite.setPosition(sf::Vector2f(10, 10));
                window.draw(gameSprite);
                window.display();
            }
        }
    }
}
