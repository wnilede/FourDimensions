#pragma once

#include <tuple>
#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#pragma warning(pop)
#include <mutex>

#include "Constants.h"
#include "Linjear4D.h"

struct Player
{
public:
	Vector4 position{ 0, 0, 0, 0 };
	Vector4 forward{ 1, 0, 0, 0 };
	FPN movementSpeed = 5;
	std::vector<Vector4> shape;
	FPN verticalSpeed = 0;
	FPN jumpSpeed = 5;
	Player();
	Vector4 GetScreenCenter() const;
	Vector4 GetScreenXVector() const;
	Vector4 GetScreenYVector() const;
	void HandleViewControlls(FPN forwardInputHorizontal, FPN forwardInputVertical, FPN pitchInput);
private:
	FPN pitch = (FPN)-0.05794838294029739848302;
	Vector4 screenUp{ 0, 1, 0, 0 };
	FPN cameraZoom = (FPN)0.5;
};
struct RayCastResult
{
	FPN distance;
	sf::Color color;
	RayCastResult(FPN distance, sf::Color color);
	bool Success();
};
enum class ColorSheme
{
	simple, dragedCubes, gradual
};
struct Colorization
{
	unsigned color1;
	unsigned color2;
	ColorSheme colorScheme;
	Colorization(sf::Color color);
	Colorization(unsigned color);
	Colorization(ColorSheme colorSheme, sf::Color color1, sf::Color color2);
	Colorization(ColorSheme colorSheme, unsigned color1, unsigned color2);
};
struct Visible
{
	int id = -1;
	Visible(int id);
};
struct Space3D : Visible
{
	Vector4 norm{ 0, 0, 0, 1 };
	Vector4 position{ 0, 0, 0, 0 };
	unsigned color = 0x00ff00ff;
	static const int id = 1;
	Space3D(Vector4 norm, Vector4 position, sf::Color color);
	FPN RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const;
};
struct Tetrahedron : Visible
{
	Vector4 position;
	Vector4 corners[3];
	Colorization colorization;
	bool actuallyParallelepiped = false;
	static const int id = 2;
	Tetrahedron(Vector4 corners[4], Colorization colorization, bool actuallyParallelepiped = false);
	Tetrahedron(Vector4 corner1, Vector4 corner2, Vector4 corner3, Vector4 corner4, Colorization colorization, bool actuallyParallelepiped = false);
	FPN RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const;
	Tetrahedron GetRotated(const Rotation rotation) const;
	void Rotate(const Rotation rotation);
};
struct Updatable
{
	virtual void Update() = 0;
};
struct Mesh : Visible
{
private:
	Vector4 position{};
	Rotation rotation{};
	std::vector<Tetrahedron> relativeTetrahedrons{};
	std::vector<Tetrahedron> changeableAbsoluteTetrahedrons;
	void UpdateTetrahedrons();
public:
	static const int id = 3;
	const std::vector<Tetrahedron>& absoluteTetrahedrons;
	void setPosition(Vector4 value);
	void setRotation(Rotation value);
	void setTetrahedrons(std::vector<Tetrahedron> tetrahedrons);
	const Vector4& getPosition();
	const Rotation& getRotation();
	const std::vector<Tetrahedron>& getTetrahedrons();
	Mesh();
	Mesh(Vector4 position, Rotation rotation, std::vector<Tetrahedron> tetrahedrons);
	Mesh(const Mesh& other);
	Mesh(const Mesh&& other) noexcept;
	FPN RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const;
	static Mesh GetCuboid(Vector4 position, Rotation rotation, Vector4 size, Colorization colorization);
	static Mesh GetCube(Vector4 position, Rotation rotation, FPN size, Colorization colorization);
};
struct RotatingMesh : Mesh, Updatable
{
	FPN rotationSpeedAround;
	FPN rotationSpeedInside;
	Vector4 vectorRotation1;
	Vector4 vectorRotation2;
	Rotation initialRotation;
	const sf::Clock& clock;
	RotatingMesh(Vector4 position, Rotation initialRotation, std::vector<Tetrahedron> tetrahedrons, const sf::Clock& clock, Vector4 vectorRotation1, Vector4 vectorRotation2, FPN rotationSpeedAround, FPN rotationSpeedInside = 0);
	RotatingMesh(Mesh mesh, const sf::Clock& clock, Vector4 vectorRotation1, Vector4 vectorRotation2, FPN rotationSpeedAround, FPN rotationSpeedInside = 0);
	void Update() override;
};
struct MovingMesh : Mesh, Updatable
{
	const std::vector<Vector4> pathCorners;
	const FPN speed;
	const FPN pathLength;
	const sf::Clock& clock;
	MovingMesh(Mesh mesh, const sf::Clock& clock, std::vector<Vector4> pathCorners, FPN speed);
	void Update() override;
private:
	static FPN GetPathLength(const std::vector<Vector4>& pathCorners);
};
enum class Sound
{
	Jump, Landing
};
struct SoundController
{
public:
	SoundController();
	SoundController(SoundController&) = delete;
	SoundController(SoundController&&) = delete;
	void PlaySoun(Sound sound, FPN volume = 100);
private:
	sf::SoundBuffer jumpBuffer;
	sf::SoundBuffer landingBuffer;
	static unsigned const maxSounds = 256; //Max amount of sounds played at once. May be different on different OS.
	sf::Sound currentSounds[maxSounds];
};
struct VisiblesImage
{
	std::vector<Visible*> visibles;
	Player player;
	VisiblesImage(const std::vector<Visible*> visibles, Player player);
	VisiblesImage(VisiblesImage&) = delete;
	VisiblesImage(VisiblesImage&&) = delete;
	~VisiblesImage();
};
