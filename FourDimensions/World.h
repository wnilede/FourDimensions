#pragma once

#include "Physics4D.h"
#include "RayCasting.cuh"
#include <mutex>

struct World
{
public:
	Player player{};
	std::vector<Visible*> visibles{};
	std::vector<Updatable*> updatables{};
	const FPN mouseScrollSensitivity;
	const FPN mouseMoveSensitivity;
	FPN gravitationalAcceleration;
	const sf::Time targetUpdateInterval = sf::seconds(1 / (FPN)60);
	sf::Clock lifetimeClock;
	std::atomic<bool> exiting = false;
	std::atomic<bool> closing = false;
	World();
	World(World& other) = delete;
	World(World&& other) = delete;
	void Run(sf::RenderWindow& window);
protected:
	void SetGoalPosition(Vector4 position);
private:
	bool showDebugInfo = false;
	SoundController soundController;
	sf::Font font;
	sf::Text text;
	sf::Clock physicsClock;
	sf::Clock graphicsClock;
	sf::Time timePerFrame;
	sf::Time timePerUpdate;
	std::mutex visiblesPlayerMutex;
	std::mutex debugInfoMutex;
	RotatingMesh goal;
	void StartDrawLoop(sf::RenderWindow& window, RayCaster& rayCaster);
	void StartPhysicsLoop(sf::RenderWindow& window, RayCaster& rayCaster);
	void UpdatePhysics(sf::RenderWindow& window, RayCaster& rayCaster);
};
struct TestingWorld : World
{
	TestingWorld();
	Space3D ground{ Vector4(0, 0, 0, 1), Vector4(), sf::Color::Green };
	Space3D wall1{ Vector4(0, 0, 1, 0), Vector4(0, 0, 1, 0), sf::Color::Red };
	Space3D wall2{ Vector4(0, 1, 0, 0), Vector4(0, 1, 0, 0), sf::Color::Yellow };
	Tetrahedron tetrahedron1{ Vector4(-1, 1, 1, 1), Vector4(1, 1, -1, 1), Vector4(1, 1, 1, -1), Vector4(-1, 1, -1, -1), Colorization(ColorSheme::simple, sf::Color::Black, sf::Color::Black) };
	Tetrahedron tetrahedron2{ Vector4(2, 11, 1, 1), Vector4(1, 11, 2, 1), Vector4(1, 11, 1, -1), Vector4(-1, 11, 2, -1), Colorization(ColorSheme::simple, sf::Color::White, sf::Color::White) };
	Tetrahedron tetrahedron3{ Vector4(-1, 1, 11, 1), Vector4(1, 1, 9, 1), Vector4(1, 1, 11, -1), Vector4(-1, 1, 9, -1), Colorization(ColorSheme::simple, sf::Color::Magenta, sf::Color::Magenta) };
	Tetrahedron parallelepepid{ Vector4(2, 0, 0, 2), Vector4(3, 0, 0, 2), Vector4(2, 1, 0, 2), Vector4(2, 0, 1, 2), Colorization(ColorSheme::simple, sf::Color::Cyan, sf::Color::Cyan), TetrahedronType::Parallelepiped };
	Mesh cube4D = Mesh::GetCube(Vector4(0, 3, 0, 1), Rotation{}, 1, Colorization(ColorSheme::rough, sf::Color(160, 160, 160), sf::Color(150, 150, 150)));
	Mesh cube4D2 = Mesh::GetCube(Vector4(0, 6, 0, 0),
		Rotation{ Vector4{ 0, 0, 0, 1 }, Vector4{ 0, 1, 0, 0 }, (FPN)constants::pi / 4 },
		2, Colorization(ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White));
	Mesh cubeoid4D = Mesh::GetCuboid(Vector4(0, 0, 8, 0),
		Rotation{ },
		Vector4(3, 2, 2, 2), Colorization(ColorSheme::gradual, sf::Color::Red, sf::Color::Blue));
	Mesh pillar = Mesh::GetCuboid(Vector4(0, 8, -6, 10),
		Rotation{ Vector4{ 0, 1, 0, 0}, Vector4{ 0, 0, 1, 0 }, (FPN)-constants::pi / 2 },
		Vector4(10, 3, 3, 3), Colorization(ColorSheme::gradual, sf::Color::Black, sf::Color::White));
	Mesh longRamp = Mesh::GetCuboid(Vector4{ 10, 0, 0, 5 },
		Rotation{ Vector4{ 1, 0, 0, 0}, Vector4{ 0, 1, 0, 0 }, (FPN)constants::pi / 4 },
		Vector4{ 3, 3, 3, 10 }, Colorization{ ColorSheme::dragedCubes, sf::Color(0, 50, 200), sf::Color::Red });
	RotatingMesh rotatingCuboid{
		Mesh::GetCuboid(Vector4{ -10, 0, 0, 1 }, Rotation{ }, Vector4{ 1, 5, 1, 1 }, Colorization{ ColorSheme::rough, 0xA04FB4FFu, 0x7DD801FF }),
		lifetimeClock, Vector4{ 1, 0, 0, 0 }, Vector4{ 0, 0, 0, 1 }, (FPN)constants::pi / 4 };
	RotatingMesh rotatingCuboid2{
		Mesh::GetCuboid(Vector4{ 0, -10, 0, 3 }, Rotation{ }, Vector4{ 1, 5, 1, 1 }, Colorization{ ColorSheme::gradual, 0xA04FB4FFu, 0x7DD801FF }),
		lifetimeClock, Vector4{ 1, 0, 0, 0 }, Vector4{ 0, 0, 1, 0 }, (FPN)constants::pi / 4 };
	MovingMesh movingMesh{ Mesh::GetCube(Vector4{ }, Rotation{ }, 1, Colorization{ sf::Color::Yellow }), lifetimeClock, 1 };
	MovingMesh elevator{ Mesh::GetCube(Vector4{ }, Rotation{ }, (FPN)2, Colorization{ ColorSheme::rough, 0x000000FFu, 0x808080FF }), lifetimeClock, 2 };
	Mesh ramp{ Mesh::GetRamp(Vector4{ 20, -2, 4, 0 }, Rotation{ }, Vector4{ 2, 2, 2, 2 }, Colorization{ ColorSheme::dragedCubes, sf::Color::White, sf::Color::Black}) };
};
struct JumpTutorialWorld : World
{
	JumpTutorialWorld();
	Space3D ground{ Vector4(0, 0, 0, 1), Vector4(), sf::Color::Green };
	Mesh surroundingCube = Mesh::GetCuboid(Vector4(4, 4, 4, 8), Rotation(Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), (FPN)constants::pi / 2), Vector4(8, 4, 4, 4), Colorization(ColorSheme::gradual, sf::Color::Green, sf::Color::Blue));
	Mesh platform1 = Mesh::GetCuboid(Vector4(6, 1, 1, (FPN)0.8), Rotation{ }, Vector4(2, 1, 1, 1), Colorization(sf::Color::Black));
	Mesh platform2 = Mesh::GetCube(Vector4(7, 3, 1, (FPN)2.6), Rotation{ }, 1, Colorization(sf::Color::Black));
	Mesh platform3 = Mesh::GetCuboid(Vector4(7, 6, 1, (FPN)4.4), Rotation{ }, Vector4(1, 2, 1, 1), Colorization(sf::Color::Black));
	Mesh platform4 = Mesh::GetCuboid(Vector4(3, 7, 1, (FPN)6.2), Rotation{ }, Vector4(3, 1, 1, 1), Colorization(sf::Color::Black));
	Mesh platform5 = Mesh::GetCube(Vector4(1, 7, 3, 8), Rotation{ }, 1, Colorization(sf::Color::Black));
	Mesh platform6 = Mesh::GetCuboid(Vector4(1, 7, 6, (FPN)9.8), Rotation{ }, Vector4(1, 1, 2, 1), Colorization(sf::Color::Black));
	Mesh platform7 = Mesh::GetCuboid(Vector4(5, 7, 7, (FPN)11.6), Rotation{ }, Vector4(3, 1, 1, 1), Colorization(sf::Color::Black));
};
