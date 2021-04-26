#include<SFML/Graphics.hpp>
#include<iostream>
#include<thread>
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

    sf::Vector2f m_pos = sf::Vector2f(50,50);

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

        sf::Vector2f last_m_pos = m_pos;
        m_pos = sf::Vector2f(sf::Mouse::getPosition(window));
        m_pos = sf::Vector2f((int)m_pos.x / 50 * 50, (int)m_pos.y / 50 * 50);
        if (last_m_pos.x != m_pos.x || last_m_pos.y != m_pos.y)
        {
            mouseUpdate = true;
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

                for (int i = 0; i < activeChunk->chunk.size(); i++)
                {
                    activeChunk->chunk[i]->Update(deltatime);
                }
                for (int i = 0; i < 100; i += 10)
                {
                    for (int j = 0; j < 10; j++)
                    {
                        window.draw(activeChunk->chunk[static_cast<__int64>(i) + j]->Draw(sf::Vector2f(j * 50, i * 5)));
                    }
                }

                sf::CircleShape circ;
                circ.setFillColor(sf::Color::Black);
                circ.setRadius(25.0f);
                circ.setPosition(sf::Vector2f(this->player.playerPos.x * 50, this->player.playerPos.y * 50));
                window.draw(circ);

                sf::RectangleShape mouseRect;
                mouseRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
                mouseRect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
                mouseRect.setOutlineThickness(1.0f);
                mouseRect.setPosition(m_pos);
                mouseRect.setSize(sf::Vector2f(50, 50));
                window.draw(mouseRect);

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


                for (int i = 0; i < activeChunk->chunk.size(); i++)
                {
                    activeChunk->chunk[i]->Update(deltatime);
                }

                window.clear();

                for (int i = 0; i < 100; i += 10)
                {
                    for (int j = 0; j < 10; j++)
                    {
                        window.draw(activeChunk->chunk[static_cast<__int64>(i) + j]->Draw(sf::Vector2f(j * 50, i * 5)));
                    }
                }

                sf::CircleShape circ;
                circ.setFillColor(sf::Color::Black);
                circ.setRadius(25.0f);
                circ.setPosition(sf::Vector2f(this->player.playerPos.x * 50, this->player.playerPos.y * 50));
                window.draw(circ);

                sf::RectangleShape mouseRect;
                mouseRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
                mouseRect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
                mouseRect.setOutlineThickness(1.0f);
                mouseRect.setPosition(m_pos);
                mouseRect.setSize(sf::Vector2f(50, 50));
                window.draw(mouseRect);


                /*if (this->isFontReady)
                {
                    sf::Text location;
                    location.setFont(this->font);

                    location.setString();
                    location.setPosition(sf::Vector2f(10, 10));

                    window.draw(location);
                }*/

                window.display();
            }
        }
    }
}


