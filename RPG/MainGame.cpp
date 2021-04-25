#include<SFML/Graphics.hpp>
#include<iostream>
#include<thread>
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

    this->playerPos = sf::Vector2f(0, 0);
    
    MapReader reader = MapReader();
    Chunk* activeChunk = new Chunk();
    activeChunk->chunk = reader.getMapChunk(MAP_PATH, this->playerPos);
    
    bool HasUpdate = true;
    
    int start = time(0);
    int current = time(0);
    int frames = 0;

    sf::Vector2f m_pos = sf::Vector2f(50,50);

    while (window.isOpen())
    {
        deltatime = clock.restart().asSeconds();

        
        frames++;
        current = time(0);
        if (current - start >= 1)
        {
            window.setTitle(std::to_string(frames));
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
       
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            HasUpdate = true;
        }



        sf::Vector2f last_m_pos = m_pos;
        m_pos = sf::Vector2f(sf::Mouse::getPosition(window));
        m_pos = sf::Vector2f(((int)(m_pos.x / 50) * 50), ((int)(m_pos.y / 50) * 50));
        if (last_m_pos.x != m_pos.x || last_m_pos.y != m_pos.y)
        {
            HasUpdate = true;
        }

       

        

        if (HasUpdate)
        {
            HasUpdate = false;
            for (int i = 0; i < activeChunk->chunk.size(); i++)
            {
                activeChunk->chunk[i]->Update(deltatime);
            }

            window.clear();

            for (int i = 0; i < 100; i += 10)
            {
                for (int j = 0; j < 10; j++)
                {
                    window.draw(activeChunk->chunk[i + j]->Draw(sf::Vector2f(j * 50, i * 5)));
                }
            }

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


