#include "Physics4D.h"

FPN RayCast(std::vector<Visible*>& visibles, Vector4 rayOrigin, Vector4 RayDirection)
{
	FPN closestDistance = std::numeric_limits<FPN>::infinity();
	for (Visible* visible : visibles)
	{
		FPN distance;
		switch (visible->id)
		{
		case Space3D::id:
			distance = static_cast<Space3D*>(visible)->RayCast(rayOrigin, RayDirection);
			break;
		case Tetrahedron::id:
			distance = static_cast<Tetrahedron*>(visible)->RayCast(rayOrigin, RayDirection);
			break;
		case Mesh::id:
			distance = static_cast<Mesh*>(visible)->RayCast(rayOrigin, RayDirection);
			break;
		default:
			throw std::exception(("Unknown visible id '" + std::to_string(visible->id) + "'.").c_str());
		}
		if (distance >= 0 && distance < closestDistance)
			closestDistance = distance;
	}
	return closestDistance;
}
