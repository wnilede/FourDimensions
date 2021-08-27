#include "Physics4D.h"

Mesh::Mesh() : Visible(id), absoluteTetrahedrons(changeableAbsoluteTetrahedrons)
{ }
Mesh::Mesh(Vector4 position, Rotation rotation, std::vector<Tetrahedron> tetrahedrons) :
	Visible(id), position(position), rotation(rotation), relativeTetrahedrons(tetrahedrons), absoluteTetrahedrons(changeableAbsoluteTetrahedrons)
{
	UpdateTetrahedrons();
}
Mesh::Mesh(const Mesh& other) : Visible(id),
position(other.position), rotation(other.rotation), relativeTetrahedrons(other.relativeTetrahedrons), changeableAbsoluteTetrahedrons(other.changeableAbsoluteTetrahedrons), absoluteTetrahedrons(changeableAbsoluteTetrahedrons)
{ }
Mesh::Mesh(const Mesh&& other) noexcept : Visible(id),
position(other.position), rotation(other.rotation), relativeTetrahedrons(other.relativeTetrahedrons), changeableAbsoluteTetrahedrons(other.changeableAbsoluteTetrahedrons), absoluteTetrahedrons(changeableAbsoluteTetrahedrons)
{ }
void Mesh::operator=(const Mesh& other)
{
	position = other.position;
	rotation = other.rotation;
	relativeTetrahedrons = other.relativeTetrahedrons;
	changeableAbsoluteTetrahedrons = other.changeableAbsoluteTetrahedrons;
}
void Mesh::setTetrahedrons(std::vector<Tetrahedron> value)
{
	this->relativeTetrahedrons = value;
	UpdateTetrahedrons();
}
void Mesh::setPosition(Vector4 value)
{
	position = value;
	UpdateTetrahedrons();
}
void Mesh::setRotation(Rotation value)
{
	rotation = value;
	UpdateTetrahedrons();
}
const Vector4& Mesh::getPosition()
{
	return position;
}
const Rotation& Mesh::getRotation()
{
	return rotation;
}
const std::vector<Tetrahedron>& Mesh::getTetrahedrons()
{
	return relativeTetrahedrons;
}
FPN Mesh::RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const
{
	FPN minimalDistance = std::numeric_limits<FPN>().infinity();
	for (const Tetrahedron& tetrahedron : absoluteTetrahedrons)
	{
		FPN distance = tetrahedron.RayCast(RayOrigin, RayDirection);
		if (distance > 0 && distance < minimalDistance)
			minimalDistance = distance;
	}
	return minimalDistance;
}
void Mesh::UpdateTetrahedrons()
{
	changeableAbsoluteTetrahedrons = relativeTetrahedrons;
	for (int i = 0; i < relativeTetrahedrons.size(); i++)
	{
		this->changeableAbsoluteTetrahedrons[i].Rotate(rotation);
		this->changeableAbsoluteTetrahedrons[i].position += position;
	}
}
Mesh Mesh::GetCuboid(Vector4 position, Rotation rotation, Vector4 size, Colorization colorization)
{
	std::vector<Tetrahedron> tetrahedrons{};
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, size.Y, size.Z, size.W),
		Vector4(-size.X, size.Y, size.Z, size.W),
		Vector4(size.X, -size.Y, size.Z, size.W),
		Vector4(size.X, size.Y, -size.Z, size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, size.Y, size.Z, -size.W),
		Vector4(-size.X, size.Y, size.Z, -size.W),
		Vector4(size.X, -size.Y, size.Z, -size.W),
		Vector4(size.X, size.Y, -size.Z, -size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, size.Y, size.Z, size.W),
		Vector4(-size.X, size.Y, size.Z, size.W),
		Vector4(size.X, -size.Y, size.Z, size.W),
		Vector4(size.X, size.Y, size.Z, -size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, size.Y, -size.Z, size.W),
		Vector4(-size.X, size.Y, -size.Z, size.W),
		Vector4(size.X, -size.Y, -size.Z, size.W),
		Vector4(size.X, size.Y, -size.Z, -size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, size.Y, size.Z, size.W),
		Vector4(-size.X, size.Y, size.Z, size.W),
		Vector4(size.X, size.Y, -size.Z, size.W),
		Vector4(size.X, size.Y, size.Z, -size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, -size.Y, size.Z, size.W),
		Vector4(-size.X, -size.Y, size.Z, size.W),
		Vector4(size.X, -size.Y, -size.Z, size.W),
		Vector4(size.X, -size.Y, size.Z, -size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(size.X, size.Y, size.Z, size.W),
		Vector4(size.X, -size.Y, size.Z, size.W),
		Vector4(size.X, size.Y, -size.Z, size.W),
		Vector4(size.X, size.Y, size.Z, -size.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(-size.X, size.Y, size.Z, size.W),
		Vector4(-size.X, -size.Y, size.Z, size.W),
		Vector4(-size.X, size.Y, -size.Z, size.W),
		Vector4(-size.X, size.Y, size.Z, -size.W),
		colorization, true));
	switch (colorization.colorScheme)
	{
	case ColorSheme::gradual:
		tetrahedrons[6].colorization = Colorization{ ColorSheme::simple, colorization.color2, colorization.color2 };
		tetrahedrons[7].colorization = Colorization{ ColorSheme::simple, colorization.color1, colorization.color1 };
		break;
	}
	return Mesh{ position, rotation, tetrahedrons };
}
Mesh Mesh::GetCube(Vector4 position, Rotation rotation, FPN size, Colorization colorization)
{
	return GetCuboid(position, rotation, Vector4{ size, size, size, size }, colorization);
}
