#include "Physics4D.h"

Cube4D::Cube4D(Vector4 position, Rotation rotation, FPN size, Colorization colorization) :
	Cuboid4D{ position, rotation, Vector4{ size, size, size, size }, colorization }
{ }