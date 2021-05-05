#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <json/json.h>
#include <thread>
#include <Windows.h>

#include "MainGame.h"
#include "GameAudio.h"
#include "Log.h"
#include "Utils.h"


MainGame::MainGame()
{
    char* EXE_FOLDER = new char[MAX_PATH];
    GetModuleFileNameA(NULL, EXE_FOLDER, std::ios::binary);
    std::string pfont = std::string(EXE_FOLDER) + "\\assets\\Fonts\\Arial\\arial.ttf";
    std::string pmap = std::string(EXE_FOLDER) + "\\assets\\Maps\\map.json";
    delete[] EXE_FOLDER;

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


int MainGame::Run()
{
    sf::RenderWindow window(sf::VideoMode(800, 500), "Cube", sf::Style::Close);

    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(0);

    bool chunkChange = false;
    bool endAnimation = false;
    bool mapReady = false;
    bool gameGlobalUpdate = false;
    bool restart = false;

    float chunkChangeAnimation = chunkAnimationDuration;
    float deltatime = 0;
    int frames = 0;
    float gameTime = 0.0f;

    time_t start = time(0);
    time_t current = time(0);

    sf::Clock clock;
    sf::Event event;
    sf::RenderTexture gameTexture;
    std::vector<Chunk*> map;

    Chunk* activeChunk = new Chunk();
    MouseUpdate mouseUpdate;
    ChunkUpdate chunkUpdate;
    PlayerUpdate playerUpdate;
    GameTexture textures = GameTexture();
    GameAudio sounds = GameAudio();
    HUD hud = HUD(this->font);

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
            while (!reader.isTexturesLoad) {}

            map = reader.getMap();
            if (map.size() == 0)
            {
                HasError = true;
                errorValue = "[ERROR] Too many chunks, please do not exceed 1500 chunk.";
            }
            else
            {
                textures.block_textures = reader.block_textures;

                textures.enemies_textures = reader.enemies_textures;

                textures.items_textures = reader.items_textures;

                textures.players_textures = reader.players_textures;

                sounds.sounds = reader.sounds;
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

                if (HasError || playerUpdate.isDead)
                {
                    if (playerUpdate.isDead)
                    {
                        window.clear();
                        sf::Text dead;
                        dead.setFont(font);
                        dead.setCharacterSize(20);

                        dead.setString("Vous êtes mort... Appuyer sur [SPACE] pour recommencer ou [ESCAPE] pour quitter.");
                        
                        dead.setPosition(sf::Vector2f(20, (500 / 2) - 20));
                        dead.setFillColor(sf::Color::Red);

                        window.draw(dead);
                        window.display();

                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                        {
                            restart = true;
                            window.close();
                        }
                    }else
                    {
                        LOG() << "[game error] " << errorValue;
                        
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
                        }
                        else
                        {
                            window.setTitle(errorValue);
                        }
                    }
                }
                else
                {
                    deltatime = clock.restart().asSeconds();
                    gameTime += deltatime;

                    frames++;
                    current = time(0);
                    if (current - start >= (time_t)1)
                    {
                    #ifdef _DEBUG
                        window.setTitle(std::to_string(frames));
                    #endif
                        gameGlobalUpdate = true;
                        frames = 0;
                        start = time(0);
                    }

                    if (gameTime >= gameTimeSwitch)
                    {
                        gameTime = 0.0f;
                        LOG() << "[time] day night cycle";
                        isNight = !isNight;
                        playerUpdate.hasUpdate = true;
                    }

                    if (mapReady)
                    {
                        PlayerObject pO;
                        pO.isRight = this->player.isDirectionR;
                        pO.isCTRL = this->player.isCTRL;
                        pO.position = this->player.playerPos;
                        pO.state = this->player.activeState;
                        pO.isNight = this->isNight;

                        chunkUpdate = activeChunk->Update(deltatime, pO, activeChunk->chunk);
                        
                        if (chunkUpdate.ItemUpdate && chunkUpdate.ItemsCollect.size() != 0)
                        {
                            for (std::pair<ItemTypes, sf::Vector2f>& it : chunkUpdate.ItemsCollect)
                            {
                                Item* item = new Item();
                                item->type = it.first;
                                item->position = it.second;

                                this->player.inventory.push_back(item);
                            }
                            
                            std::vector<int> removelist;
                            
                            for (size_t i = 0; i < activeChunk->listItems.size(); i++)
                            {
                                for (std::pair<ItemTypes, sf::Vector2f>& it : chunkUpdate.ItemsCollect)
                                {
                                    if (it.second == activeChunk->listItems[i]->position)
                                    {
                                        this->player.xp++;
                                        removelist.push_back(i);
                                        break;
                                    }
                                }
                            }

                            if (removelist.size() != 0)
                            {
                                for (size_t i = 0; i < removelist.size(); i++)
                                {
                                    activeChunk->listItems.erase(activeChunk->listItems.begin() + removelist[i] - i);
                                }

                                int id = getChunkId(map, activeChunk);
                                map[id]->size = activeChunk->size;
                                map[id]->listEnemies = activeChunk->listEnemies;
                                map[id]->listItems = activeChunk->listItems;
                                map[id]->chunk = activeChunk->chunk;
                                map[id]->isNight = activeChunk->isNight;
                                map[id]->position = activeChunk->position;
                            }
                            removelist.clear();
                            chunkUpdate.ItemsCollect.clear();
                        }
                        
                        playerUpdate = this->player.KeyBoardUpdate(deltatime, activeChunk,this->isNight,sounds);
                        
                        if (playerUpdate.hasChunkUpdate)
                        {
                            activeChunk = playerUpdate.chunkUpdate;
                            int id = getChunkId(map, activeChunk);

                            map[id]->size = activeChunk->size;
                            map[id]->listEnemies = activeChunk->listEnemies;
                            map[id]->listItems = activeChunk->listItems;
                            map[id]->chunk = activeChunk->chunk;
                            map[id]->isNight = activeChunk->isNight;
                            map[id]->position = activeChunk->position;
                        }

                        if (playerUpdate.hasUpdate)
                        {
                            sf::Vector2f cubePlayerPos = sf::Vector2f(round(this->player.playerPos.x), round(this->player.playerPos.y));
                            if (this->player.isCollisionBlock(getCaseByPosition(activeChunk->chunk, cubePlayerPos)->case_type))
                            {
                                this->player.goForward();
                            }

                            chunkChange = this->player.MapUpdate(deltatime, map, activeChunk,sounds);
                        }

                        mouseUpdate = this->mouse.Update(window);

                        /*
                        for (int i = 0; i < activeChunk->chunk.size(); i++)
                        {
                            activeChunk->chunk[i]->Update(deltatime);
                        }
                        */

                        HUDupdateObject hudObj;
                        hudObj.chunkPos = activeChunk->position;
                        hudObj.playerPos = this->player.playerPos;
                        hudObj.enemiesCount = activeChunk->listEnemies.size();
                        hudObj.inventoryItemsCount = this->player.inventory.size();
                        hudObj.activeBarIndex = this->player.HUDbarIndex;
                        hudObj.playerLife = this->player.life;
                        hudObj.playerXP = this->player.xp;
                        hudObj.playerKill = this->player.kill;
                        hudObj.deltatime = deltatime;
                        hudObj.night = (gameTime * 100 / this->gameTimeSwitch);
                        for (size_t i = 0; i < 8; i++)
                        {
                            hudObj.activeBar[i] = this->player.activeBar[i];
                        }
                        hud.Update(hudObj);
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

                            this->DrawNightFilter(gameTexture, window.getSize(), gameTime);

                            gameTexture.display();

                            window.clear();
                            sf::Sprite gameSprite(gameTexture.getTexture());
                            gameSprite.setPosition(sf::Vector2f(0, 0));
                            window.draw(gameSprite);

                            hud.Draw(window, gameSprite, textures);

                            if (this->isFontReady)
                            {
                                this->DrawText(window, "(" + std::to_string((int)activeChunk->position.x) + ";" + std::to_string((int)activeChunk->position.y) + ")", sf::Vector2f(10, 10));
                            }

                            window.display();
                        }
                    }
                    else
                    {
                        if (playerUpdate.hasUpdate || mouseUpdate.hasMove || endAnimation || chunkUpdate.EnemyUpdate || chunkUpdate.ItemUpdate || gameGlobalUpdate)
                        {
                            gameGlobalUpdate = false;
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
                                }
                                else
                                {
                                    window.setMouseCursorVisible(true);
                                }
                            }

                            this->DrawNightFilter(gameTexture, window.getSize(), gameTime);

                            gameTexture.display();

                            endAnimation = false;
                            playerUpdate.hasUpdate = false;
                            mouseUpdate.hasMove = false;


                            window.clear();
                            sf::Sprite gameSprite(gameTexture.getTexture());
                            gameSprite.setPosition(sf::Vector2f(0, 0));
                            window.draw(gameSprite);

                            hud.Draw(window, gameSprite, textures);

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

            if (restart)
            {
                return 1;
            }

            return 0;
}

void MainGame::DrawNightFilter(sf::RenderTexture& gameTexture, sf::Vector2u size, int gameTime)
{

    int gradien = (120 * gameTime)/this->gameTimeSwitch;
    if (this->isNight)
    {
        gradien = 120 - gradien;
    }
    sf::RectangleShape filter;
    filter.setFillColor(sf::Color(0, 0, 90, gradien));
    filter.setPosition(sf::Vector2f(0, 0));
    filter.setSize(sf::Vector2f(size));
    gameTexture.draw(filter);
}

int MainGame::getChunkId(std::vector<Chunk*>& map, Chunk*& chunk)
{
    std::vector<Chunk*> chunks = map;
    for (int i = 0; i < map.size(); i++)
    {
        if (map[i]->position == chunk->position)
        {
            return i;
        }
    }
    return -1;
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