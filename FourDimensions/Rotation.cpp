#include "Linjear4D.h"
#include <cmath>
#include <stdexcept>

Rotation::Rotation() : matrix{ Matrix4::Identity() }
{ }
Rotation::Rotation(Matrix4 matrix, bool checkValidity) : matrix{ matrix }
{
	if (checkValidity) {
		Matrix4 matrixTimesTranspose = matrix * matrix.Transpose();
		for (unsigned i = 0; i < 4; i++) {
			for (unsigned q = 0; q < 4; q++) {
				if (std::abs(matrixTimesTranspose.values[i][q] - (i == q) > (FPN)0.0001)) {
					throw std::invalid_argument{ "Matrix must be orthonormal to be a valid rotation." };
				}
			}
		}
		if (matrix.Determinant() < 0) {
			throw std::invalid_argument{ "Determinant of matrix must be 1 for it to be a valid rotation." };
		}
	}
}
//Rotates around the plane spanned by vector1 and vector2 by angleAround and inside it by angleInside.
Rotation::Rotation(Vector4 vector1, Vector4 vector2, FPN angleAround, FPN angleInside)
{
	vector1.Normalize();
	vector2 -= vector2 * vector1 * vector1;
	vector2.Normalize();
	Vector4 orthogonal1{ (FPN)0.1378829192105522, (FPN)-0.9785163765231606, (FPN)0.9306721413772778, (FPN)-0.4809750068433964 };
	Vector4 orthogonal2{ (FPN)-0.7030562727603511, (FPN)-0.7592083554396249, (FPN)0.8909766385936507, (FPN)0.4953219043069777 };
	orthogonal1 -= orthogonal1 * vector1 * vector1 + orthogonal1 * vector2 * vector2;
	orthogonal1.Normalize();
	orthogonal2 -= orthogonal2 * vector1 * vector1 + orthogonal2 * vector2 * vector2 + orthogonal2 * orthogonal1 * orthogonal1;
	orthogonal2.Normalize();
	Matrix4 baseTransformation{ vector1, vector2, orthogonal1, orthogonal2 };
	Matrix4 rotationNormalBase{
		std::cos(angleInside), -std::sin(angleInside), 0, 0,
		std::sin(angleInside), std::cos(angleInside), 0, 0,
		0, 0, std::cos(angleAround), -std::sin(angleAround),
		0, 0, std::sin(angleAround), std::cos(angleAround) };
	matrix = Matrix4{ baseTransformation * rotationNormalBase * baseTransformation.Transpose() };
}
//Rotates around the plane spanned by vector1 and vector2 by angleAround.
Rotation::Rotation(Vector4 vector1, Vector4 vector2, FPN angleAround) : Rotation(vector1, vector2, angleAround, 0)
{ }
Rotation Rotation::operator+(const Rotation other) const {
	return Rotation{ other.matrix * matrix };
}
void Rotation::operator+=(const Rotation other) {
	matrix = other.matrix * matrix;
}
Rotation Rotation::operator-() const
{
	return this->Invers();
}
Rotation Rotation::operator-(const Rotation other) const
{
	return *this + other.Invers();
}
void Rotation::operator-=(const Rotation other)
{
	*this = *this - other;
}
Rotation Rotation::Invers() const
{
	return Rotation{ matrix.Transpose() };
}
