#include<SFML/Graphics.hpp>
#include<iostream>
#include<thread>
#include "MainGame.h"
#include "MapReader.h"

#define MAP_PATH "C:\\Users\\jules\\source\\repos\\RPG\\x64\\Debug\\map.txt"

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

            if (event.type == sf::Event::Resized)
            {
                /*float h = event.size.height;
                float w = event.size.width;

                view.setSize(w, h);
                window.setSize(sf::Vector2u(w, h));
                view.setCenter(w / 2, h / 2);
                window.setView(view);
                */
            }
        }
       
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
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

            window.display();
        }
    }
}


