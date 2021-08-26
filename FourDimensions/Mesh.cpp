#include "Physics4D.h"

Mesh::Mesh() : Visible(id)
{ }

Mesh::Mesh(Vector4 position, Rotation rotation, std::vector<Tetrahedron> tetrahedrons) :
	Visible(id), position(position), rotation(rotation), tetrahedrons(tetrahedrons)
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
