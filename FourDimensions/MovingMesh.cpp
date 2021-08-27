#include "Physics4D.h"

MovingMesh::MovingMesh(Mesh mesh, const sf::Clock& clock, FPN speed) : MovingMesh(mesh, clock, std::vector<Vector4>{ }, speed)
{ }
MovingMesh::MovingMesh(Mesh mesh, const sf::Clock& clock, std::vector<Vector4> pathCorners, FPN speed) : Mesh(mesh), clock(clock), pathCorners(pathCorners), speed(speed), pathLength(GetPathLength(pathCorners))
{
	if (speed < 0)
		throw std::invalid_argument("Speed must be non-negative"); //Could fix if needed
}
void MovingMesh::Update()
{
	FPN distance = std::fmod(clock.getElapsedTime().asSeconds() * speed, pathLength);
	unsigned part = 0;
	while (distance > 0)
	{
		part++;
		distance -= (pathCorners[part % pathCorners.size()] - pathCorners[(part - 1) % pathCorners.size()]).GetLength();
	}
	setPosition(pathCorners[part % pathCorners.size()] + distance * (pathCorners[part % pathCorners.size()] - pathCorners[(part - 1) % pathCorners.size()]).GetNormalized());
}
void MovingMesh::SetPathCorners(std::vector<Vector4> value)
{
	if (value.size() == 0)
		throw std::invalid_argument("Cannot set a path with zero corners");
	pathLength = GetPathLength(value);
	pathCorners = value;
}
const std::vector<Vector4>& MovingMesh::GetPathCorners() const
{
	return pathCorners;
}
const FPN& MovingMesh::GetPathLength() const
{
	return pathLength;
}
FPN MovingMesh::GetPathLength(const std::vector<Vector4>& pathCorners)
{
	FPN length = 0;
	for (unsigned i = 1; i < pathCorners.size(); i++) {
		length += (pathCorners[i] - pathCorners[i - 1]).GetLength();
	}
	if (pathCorners.size() > 1) {
		length += (pathCorners[0] - pathCorners[pathCorners.size() - 1]).GetLength();
	}
	return length;
}
