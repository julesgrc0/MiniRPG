#include<SFML/Graphics.hpp>
#include<iostream>
#include <cmath>
#include "MainGame.h"
#include "MapReader.h"
#include "Consts.h"


void draw_chunk(sf::RenderWindow& window, Chunk* activeChunk)
{
    
}

MainGame::MainGame()
{
    sf::Font font;
    if (font.loadFromFile(FONT_PATH))
    {
        this->isFontReady = true;
    }
}

MainGame::~MainGame()
{

}

bool MainGame::Init(const char* path)
{
	return true;
}

void MainGame::Run()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "",sf::Style::Close);
    sf::View view = window.getDefaultView();
    
    sf::Clock clock;
    float deltatime = 0;
    this->player.playerPos = sf::Vector2f(4, 4);
    
    MapReader reader = MapReader();
    Chunk* activeChunk = new Chunk();
    
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
        }

        keyboardUpdate = this->player.KeyBoardUpdate(deltatime);
        if (keyboardUpdate)
        {
            chunkChange = this->player.MapUpdate(deltatime, reader, activeChunk);
        }

        mouseUpdate =  this->mouse.Update(window);
        
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
               
                window.clear();

                activeChunk->Draw(window);
                this->player.Draw(window);
                this->mouse.Draw(window);

                sf::RectangleShape blur;
                int gradien = 255 - (int)((255 * chunkChangeAnimation * (100/ chunkAnimationDuration)) / 100);
                blur.setFillColor(sf::Color(0, 0, 0, gradien));
                blur.setPosition(sf::Vector2f(0, 0));
                blur.setSize(sf::Vector2f(window.getSize()));
                window.draw(blur);

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

                window.clear();

                activeChunk->Draw(window);
                this->player.Draw(window);
                this->mouse.Draw(window);

                window.display();
            }
        }
    }
}


