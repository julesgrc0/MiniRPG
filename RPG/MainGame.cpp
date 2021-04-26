#include<SFML/Graphics.hpp>
#include<iostream>
#include<thread>
#include <cmath>
#include "MainGame.h"
#include "MapReader.h"

#define MAP_PATH "C:\\Users\\jules\\source\\repos\\RPG\\x64\\Debug\\assets\\Maps\\debug.txt"

void draw_chunk(sf::RenderWindow& window, Chunk* activeChunk)
{
    
}

MainGame::MainGame()
{
   
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
    this->playerPos = sf::Vector2f(4, 4);
    
    MapReader reader = MapReader();
    Chunk* activeChunk = new Chunk();
    activeChunk->position = sf::Vector2f(0, 0);
    activeChunk->chunk = reader.getMapChunk(MAP_PATH, activeChunk->position);
    
    bool mouseUpdate = true;
    bool keyboardUpdate = true;
    bool chunkChange = false;
    
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
            window.setTitle(std::to_string(frames) + " (" + std::to_string((int)activeChunk->position.x) +";" + std::to_string((int)activeChunk->position.y)+")");
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


        int playerSeep = 10;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            playerSeep = 2;
        }else
        {
            playerSeep = 10;
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

        if (keyboardUpdate)
        {
            sf::Vector2f roundPlayer = sf::Vector2f((this->playerPos.x * 50/50),(this->playerPos.y*50/50));
            sf::Vector2f next;

            if (roundPlayer.x > 9)
            {
                next = activeChunk->position;
                next.x++;
                std::vector<Case*> c = reader.getMapChunk(MAP_PATH, next);
                if (c.size() > 0)
                {
                    this->playerPos.x = 0;
                    this->playerPos.x += playerSeep * deltatime;
                    activeChunk->chunk = c;
                    activeChunk->position = next;
                    chunkChange = true;
                }else
                {
                    this->playerPos.x -= playerSeep * deltatime;
                }
            }
            else if (roundPlayer.x < 0)
            {
                next = activeChunk->position;
                next.x--;
                std::vector<Case*> c = reader.getMapChunk(MAP_PATH, next);
                if (c.size() > 0)
                {
                    this->playerPos.x = 9;
                    this->playerPos.x -= playerSeep * deltatime;
                    activeChunk->chunk = c;
                    activeChunk->position = next;
                    chunkChange = true;
                }else
                {
                    this->playerPos.x += playerSeep * deltatime;
                }
            }

            if (roundPlayer.y > 9)
            {
                next = activeChunk->position;
                next.y++;
                std::vector<Case*> c = reader.getMapChunk(MAP_PATH, next);
                if (c.size() > 0)
                {
                    this->playerPos.y = 0;
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
            else if (roundPlayer.y < 0)
            {
                next = activeChunk->position;
                next.y--;
                std::vector<Case*> c = reader.getMapChunk(MAP_PATH, next);
                if (c.size() > 0)
                {
                    this->playerPos.y = 9;
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
        }

        sf::Vector2f last_m_pos = m_pos;
        m_pos = sf::Vector2f(sf::Mouse::getPosition(window));
        m_pos = sf::Vector2f((int)m_pos.x / 50 * 50, (int)m_pos.y / 50 * 50);
        if (last_m_pos.x != m_pos.x || last_m_pos.y != m_pos.y)
        {
            mouseUpdate = true;
        }


        if (keyboardUpdate || mouseUpdate)
        {
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
            circ.setPosition(sf::Vector2f(this->playerPos.x * 50, this->playerPos.y * 50));
            window.draw(circ);

            sf::RectangleShape rect;
            rect.setFillColor(sf::Color::Color(0, 0, 0, 0));
            rect.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
            rect.setOutlineThickness(1.0f);
            rect.setPosition(m_pos);
            rect.setSize(sf::Vector2f(50, 50));
            window.draw(rect);

            window.display();
        }
    }
}


