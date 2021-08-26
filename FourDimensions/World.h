#pragma once

#include "Physics4D.h"
#include "RayCasting.cuh"
#include <mutex>

struct World
{
public:
	Player player{};
	std::vector<Visible*> visibles{};
	const FPN mouseScrollSensitivity;
	const FPN mouseMoveSensitivity;
	FPN gravitationalAcceleration;
	const sf::Time targetUpdateInterval = sf::seconds(1 / (FPN)60);
	World();
	bool IsOpen();
	void Run();
private:
	std::atomic<bool> lockTest = false;

	std::atomic<bool> closing = false;
	sf::RenderWindow window;
	SoundController soundController;
	sf::Font font;
	sf::Text text;
	sf::Clock physicsClock;
	sf::Clock graphicsClock;
	sf::Time fps;
	sf::Time ups;
	std::mutex visiblesPlayerUpsMutex;
	RayCaster rayCaster;
	void StartDrawLoop();
	void StartPhysicsLoop();
	void UpdatePhysics();
	void Draw();
};
