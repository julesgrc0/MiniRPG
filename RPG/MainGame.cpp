#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <json/json.h>
#include <thread>
#include <Windows.h>

#include "MainGame.h"
#include "Log.h"
#include "Utils.h"


MainGame::MainGame()
{
    char* EXE_FOLDER = new char[MAX_PATH];
    GetModuleFileNameA(NULL, EXE_FOLDER, std::ios::binary);
    std::string pfont = std::string(EXE_FOLDER) + "\\assets\\Fonts\\Arial\\arial.ttf";
    std::string pmap = std::string(EXE_FOLDER) + "\\assets\\Maps\\map.json";
    delete EXE_FOLDER;

    if (this->font.loadFromFile(pfont))
    {
        isFontReady = true;
    }

    if (!reader.loadMap(pmap))
    {
        HasError = true;
        errorValue = "[ERROR] Fail to load map.json file.";
    }
}

MainGame::~MainGame()
{
}


void MainGame::Run()
{    
    sf::RenderWindow window(sf::VideoMode(800, 500), "", sf::Style::Close);
    
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(0);

    bool keyboardUpdate = true;
    bool chunkChange = false;
    bool endAnimation = false;
    bool mapReady = false;

    const float chunkAnimationDuration = 20.0f;
    float chunkChangeAnimation = chunkAnimationDuration;
    float deltatime = 0;
    int frames = 0;

    time_t start = time(0);
    time_t current = time(0);

    sf::Clock clock;
    sf::Event event;
    Chunk *activeChunk = new Chunk();
    MouseUpdate mouseUpdate;
    std::vector<Chunk*> map;
    sf::RenderTexture gameTexture;
    GameTexture textures = GameTexture();

    this->player.playerPos = sf::Vector2f(4, 4);
    activeChunk->position = sf::Vector2f(0, 0);
    
    for (size_t i = 0; i < 100; i++)
    {
        Case* item = new Case(50);
        activeChunk->chunk.push_back(item);
    }

    std::thread([&] {
        activeChunk->chunk = this->reader.getChunk(activeChunk->position);
     }).detach();
    
    std::thread([&] {
         while(!reader.isTexturesLoad) {}
         
         map = reader.getMap();
         if (map.size() == 0)
         {
             HasError = true;
             errorValue = "[ERROR] Too many chunks, please do not exceed 1500 chunk.";
         }else
         {
             textures.block_textures = reader.block_textures;
             textures.enemies_textures = reader.enemies_textures;
             textures.items_textures = reader.items_textures;
             textures.players_textures = reader.players_textures;
         }
         mapReady = true;
         chunkChangeAnimation = 0.0f;
         
    }).detach();
    

    if (activeChunk->chunk.size() == 0)
    {
        HasError = true;
        errorValue = "[ERROR] Fail to load first chunk please check the map.json file.";
    }
    
    this->player.addVisitedChunk(activeChunk->position);
    gameTexture.create(500, 500);
    
    while (window.isOpen())
    {  
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

        if (HasError)
        {
            if (this->isFontReady)
            {
                window.clear();
                sf::Text errorText;
                errorText.setFont(font);
                errorText.setString(errorValue);
                errorText.setCharacterSize(14);
                errorText.setPosition(sf::Vector2f(10, 10));
                errorText.setFillColor(sf::Color::Red);
                window.draw(errorText);
                window.display();
            }else
            {
                window.setTitle(errorValue);
            }
        }else
        {
            deltatime = clock.restart().asSeconds();

            frames++;
            current = time(0);
            if (current - start >= (time_t)1)
            {
                window.setTitle(std::to_string(frames));

                frames = 0;
                start = time(0);
            }

            if (mapReady)
            {
                keyboardUpdate = this->player.KeyBoardUpdate(deltatime,activeChunk->chunk);
                if (keyboardUpdate)
                {
                    sf::Vector2f cubePlayerPos = sf::Vector2f(round(this->player.playerPos.x), round(this->player.playerPos.y));
                    if (this->player.isCollisionBlock(getCaseByPosition(activeChunk->chunk, cubePlayerPos)->case_type))
                    {
                        this->player.goForward();
                    }

                    chunkChange = this->player.MapUpdate(deltatime, map, activeChunk);
                }

                mouseUpdate = this->mouse.Update(window);

                for (int i = 0; i < activeChunk->chunk.size(); i++)
                {
                    activeChunk->chunk[i]->Update(deltatime);
                }
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
                    activeChunk->Draw(gameTexture, textures);
                    this->player.Draw(gameTexture, textures);
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
                    gameSprite.setPosition(sf::Vector2f(0, 0));
                    window.draw(gameSprite);
                    
                    if (this->isFontReady)
                    {
                        this->DrawText(window, "(" + std::to_string((int)activeChunk->position.x) + ";" + std::to_string((int)activeChunk->position.y) + ")", sf::Vector2f(10, 10));
                    }

                   window.display();
                }
            }
            else
            {
                if (keyboardUpdate || mouseUpdate.hasMove || endAnimation)
                {

                    gameTexture.clear();

                    activeChunk->Draw(gameTexture, textures);

                    if (mapReady)
                    {
                        this->player.Draw(gameTexture, textures);
                        if (mouseUpdate.isOnMap)
                        {
                            window.setMouseCursorVisible(false);
                            this->mouse.Draw(gameTexture);
                            if (mouseUpdate.hasClick)
                            {
                                LOG() << "[mouse block] " << (int)getCaseByPosition(activeChunk->chunk, sf::Vector2f(mouseUpdate.coord.x / 50, mouseUpdate.coord.y / 50))->case_type;
                            }
                        }else
                        {
                            window.setMouseCursorVisible(true);
                        }
                    }

                    gameTexture.display();

                    endAnimation = false;
                    keyboardUpdate = false;
                    mouseUpdate.hasMove = false;


                    window.clear();
                    sf::Sprite gameSprite(gameTexture.getTexture());
                    gameSprite.setPosition(sf::Vector2f(0, 0));
                    window.draw(gameSprite);


                    if (!mapReady)
                    {
                        this->DrawText(window, "Chargement de la carte... ", sf::Vector2f(10, 10));
                    }
                    else
                    {
                        this->DrawText(window, "(" + std::to_string((int)activeChunk->position.x) + ";" + std::to_string((int)activeChunk->position.y) + ")", sf::Vector2f(10, 10));
                    }

                    window.display();
                }
            }
        }
    }

    delete activeChunk;
    map.clear();
}

void MainGame::DrawText(sf::RenderWindow& window,sf::String text,sf::Vector2f pos)
{
    if (this->isFontReady)
    {
        sf::Text t;
        t.setFont(font);
        t.setString(text);
        t.setCharacterSize(14);
        t.setPosition(pos);
        window.draw(t);
    }
}