#include "World.h"

RotatingMesh::RotatingMesh(Vector4 position, Rotation initialRotation, std::vector<Tetrahedron> tetrahedrons, const sf::Clock& clock, Vector4 vectorRotation1, Vector4 vectorRotation2, FPN rotationSpeedAround, FPN rotationSpeedInside) :
	Mesh(position, initialRotation, tetrahedrons), initialRotation(initialRotation), vectorRotation1(vectorRotation1), vectorRotation2(vectorRotation2), rotationSpeedAround(rotationSpeedAround), rotationSpeedInside(rotationSpeedInside), clock(clock)
{ }
void RotatingMesh::Update()
{
	setRotation(initialRotation + Rotation{ vectorRotation1, vectorRotation2, rotationSpeedAround * clock.getElapsedTime().asSeconds(), rotationSpeedInside * clock.getElapsedTime().asSeconds() });
}
