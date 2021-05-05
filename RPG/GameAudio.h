#pragma once
#include<SFML/Audio.hpp>
#include<iostream>


enum SoundTypes
{
	ENEMY,
	SOUND_NONE
};

class GameAudio
{
public:
	GameAudio();
	~GameAudio();
	bool getSound(int, sf::SoundBuffer*);
	std::vector<std::pair<int, sf::SoundBuffer>> sounds;

};

