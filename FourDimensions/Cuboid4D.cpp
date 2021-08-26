#include "Physics4D.h"


Cuboid4D::Cuboid4D(Vector4 position, Rotation rotation, Vector4 sizes, Colorization colorization) :
	Mesh(position, rotation, std::vector<Tetrahedron>())
{
	std::vector<Tetrahedron> tetrahedrons{};
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(-sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, -sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, -sizes.Z, sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, sizes.Y, sizes.Z, -sizes.W),
		Vector4(-sizes.X, sizes.Y, sizes.Z, -sizes.W),
		Vector4(sizes.X, -sizes.Y, sizes.Z, -sizes.W),
		Vector4(sizes.X, sizes.Y, -sizes.Z, -sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(-sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, -sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, sizes.Z, -sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, sizes.Y, -sizes.Z, sizes.W),
		Vector4(-sizes.X, sizes.Y, -sizes.Z, sizes.W),
		Vector4(sizes.X, -sizes.Y, -sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, -sizes.Z, -sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(-sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, -sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, sizes.Z, -sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, -sizes.Y, sizes.Z, sizes.W),
		Vector4(-sizes.X, -sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, -sizes.Y, -sizes.Z, sizes.W),
		Vector4(sizes.X, -sizes.Y, sizes.Z, -sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, -sizes.Y, sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, -sizes.Z, sizes.W),
		Vector4(sizes.X, sizes.Y, sizes.Z, -sizes.W),
		colorization, true));
	tetrahedrons.push_back(Tetrahedron(
		Vector4(-sizes.X, sizes.Y, sizes.Z, sizes.W),
		Vector4(-sizes.X, -sizes.Y, sizes.Z, sizes.W),
		Vector4(-sizes.X, sizes.Y, -sizes.Z, sizes.W),
		Vector4(-sizes.X, sizes.Y, sizes.Z, -sizes.W),
		colorization, true));
	setTetrahedrons(tetrahedrons);
	ApplyExtraColorization(colorization);
}
void Cuboid4D::ApplyExtraColorization(Colorization colorization)
{
	switch (colorization.colorScheme)
	{
	case ColorSheme::gradual:
		tetrahedrons[6].colorization = Colorization{ ColorSheme::simple, colorization.color2, colorization.color2 };
		tetrahedrons[7].colorization = Colorization{ ColorSheme::simple, colorization.color1, colorization.color1 };
		break;
	}
}
