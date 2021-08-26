#include "Physics4D.h"

Space3D::Space3D(Vector4 norm, Vector4 position, sf::Color color) : norm(norm.GetNormalized()), position(position), color(color.toInteger()), Visible(id) { }
FPN Space3D::RayCast(const Vector4& RayOrigin, const Vector4& RayDirection) const
{
	FPN linePlaneScalar = RayDirection.GetNormalized() * norm;
	if (linePlaneScalar != 0)
	{
		return (position - RayOrigin) * norm / linePlaneScalar;
	}
	else if ((position - RayOrigin) * norm == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}