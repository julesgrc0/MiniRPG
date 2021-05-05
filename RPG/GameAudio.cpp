#include "GameAudio.h"


GameAudio::GameAudio()
{

}

bool GameAudio::getSound(int id, sf::SoundBuffer* out)
{
	for (std::pair<int, sf::SoundBuffer>& s : this->sounds)
	{
		if (s.first == id)
		{
			(*out) = s.second;
			return true;
		}
	}
	return false;
}

GameAudio::~GameAudio()
{
	sounds.clear();
}