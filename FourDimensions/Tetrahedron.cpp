#include "Physics4D.h"

Tetrahedron::Tetrahedron(Vector4 corners[4], Colorization colorization, TetrahedronType tetrahedronType) :
	Visible(id), colorization(colorization), position(corners[0])
{
	this->corners[0] = corners[1] - position;
	this->corners[1] = corners[2] - position;
	this->corners[2] = corners[3] - position;
	this->tetrahedronType = tetrahedronType;
}
Tetrahedron::Tetrahedron(Vector4 corner1, Vector4 corner2, Vector4 corner3, Vector4 corner4, Colorization colorization, TetrahedronType tetrahedronType) :
	Visible(id), colorization(colorization), position(corner1)
{
	corners[0] = corner2 - position;
	corners[1] = corner3 - position;
	corners[2] = corner4 - position;
	this->tetrahedronType = tetrahedronType;
}
FPN Tetrahedron::RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const
{
	FPN conditionNumber;
	Vector4 result = Matrix4{ corners[0], corners[1], corners[2], RayDirection.GetNormalized() }.Invers(conditionNumber) * (RayOrigin - position);
	if (conditionNumber < 100000 && result[0] >= 0 && result[1] >= 0 && result[2] >= 0 && (
		tetrahedronType == TetrahedronType::Tetrahedron && result[0] + result[1] + result[2] <= 1 ||
		tetrahedronType == TetrahedronType::Ramp && result[0] + result[1] <= 1 && result[2] <= 1 ||
		tetrahedronType == TetrahedronType::Parallelepiped && result[0] <= 1 && result[1] <= 1 && result[2] <= 1))
		return -result[3];
	else
		return -1;
}
Tetrahedron Tetrahedron::GetRotated(const Rotation rotation) const {
	Tetrahedron copy{ *this };
	copy.Rotate(rotation);
	return copy;
}
void Tetrahedron::Rotate(const Rotation rotation) {
	position.Rotate(rotation);
	corners[0].Rotate(rotation);
	corners[1].Rotate(rotation);
	corners[2].Rotate(rotation);
}
