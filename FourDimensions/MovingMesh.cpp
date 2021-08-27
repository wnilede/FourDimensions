#include "Physics4D.h"

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
FPN MovingMesh::GetPathLength(const std::vector<Vector4>& pathCorners)
{
	if (pathCorners.size() < 2)
		throw std::invalid_argument("Path must contain at least 2 corners"); //Could fix if needed
	FPN length = 0;
	for (unsigned i = 1; i < pathCorners.size(); i++)
	{
		length += (pathCorners[i] - pathCorners[i - 1]).GetLength();
	}
	length += (pathCorners[0] - pathCorners[pathCorners.size() - 1]).GetLength();
	return length;
}
