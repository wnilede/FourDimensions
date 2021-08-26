#include "Physics4D.h"

Mesh::Mesh() : Visible(id)
{ }
Mesh::Mesh(Vector4 position, Rotation rotation, std::vector<Tetrahedron> tetrahedrons) :
	Visible(id), position(position), rotation(rotation), relativeTetrahedrons(tetrahedrons)
{
	UpdateTetrahedrons();
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
FPN Mesh::RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const
{
	FPN minimalDistance = std::numeric_limits<FPN>().infinity();
	for (const Tetrahedron& tetrahedron : tetrahedrons)
	{
		FPN distance = tetrahedron.RayCast(RayOrigin, RayDirection);
		if (distance > 0 && distance < minimalDistance)
			minimalDistance = distance;
	}
	return minimalDistance;
}
void Mesh::UpdateTetrahedrons()
{
	tetrahedrons = relativeTetrahedrons;
	for (int i = 0; i < relativeTetrahedrons.size(); i++)
	{
		this->tetrahedrons[i].Rotate(rotation);
		this->tetrahedrons[i].position += position;
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
