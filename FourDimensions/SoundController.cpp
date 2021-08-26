#include "Physics4D.h"
#include <iostream>

SoundController::SoundController()
{
	if (!jumpBuffer.loadFromFile("sound/jump.wav"))
		std::cout << "jump.wav not found";
	if (!landingBuffer.loadFromFile("sound/landing.wav"))
		std::cout << "landing.wav not found";
}
void SoundController::PlaySoun(Sound sound, FPN volume)
{
	for (unsigned i = 0; i < maxSounds; i++)
	{
		if (currentSounds[i].getStatus() != sf::SoundSource::Status::Playing)
		{
			switch (sound)
			{
			case Sound::Jump:
				currentSounds[i].setBuffer(jumpBuffer);
				break;
			case Sound::Landing:
				currentSounds[i].setBuffer(landingBuffer);
				break;
			}
			if (volume >= 0 && volume < 100)
				currentSounds[i].setVolume((float)volume);
			currentSounds[i].play();
			break;
		}
	}
}
