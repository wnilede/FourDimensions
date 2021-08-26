#include "Linjear4D.h"
#include <cmath>

Vector4::Vector4(FPN x, FPN y, FPN z) : X(x), Y(y), Z(z), W() { }
Vector4::Vector4(FPN x, FPN y, FPN z, FPN w) : X(x), Y(y), Z(z), W(w) { }
bool Vector4::operator==(const Vector4 other) const
{
	return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
}
bool Vector4::operator!=(const Vector4 other) const
{
	return X != other.X || Y != other.Y || Z != other.Z || W != other.W;
}
Vector4 Vector4::operator+(const Vector4 other) const
{
	return { X + other.X, Y + other.Y, Z + other.Z, W + other.W };
}
void Vector4::operator+=(const Vector4 other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	W += other.W;
}
Vector4 Vector4::operator-(const Vector4 other) const
{
	return { X - other.X, Y - other.Y, Z - other.Z, W - other.W };
}
void Vector4::operator-=(const Vector4 other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	W -= other.W;
}
Vector4 Vector4::operator-() const
{
	return { -X, -Y, -Z, -W };
}
Vector4 operator*(const FPN scalar, const Vector4 vector)
{
	return { scalar * vector.X, scalar * vector.Y, scalar * vector.Z, scalar * vector.W };
}
Vector4 operator*(const Vector4 vector, const FPN scalar)
{
	return { vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar };
}
void operator*=(Vector4& vector, const FPN scalar)
{
	vector.X *= scalar;
	vector.Y *= scalar;
	vector.Z *= scalar;
	vector.W *= scalar;
}
Vector4 operator/(const Vector4 vector, const FPN scalar)
{
	return { vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar };
}
void operator/=(Vector4& vector, const FPN scalar)
{
	vector.X /= scalar;
	vector.Y /= scalar;
	vector.Z /= scalar;
	vector.W /= scalar;
}
FPN Vector4::operator*(const Vector4 other) const
{
	return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
}
FPN& Vector4::operator[](const int coordinate)
{
	return (&X)[coordinate];
}
FPN Vector4::operator[](const int coordinate) const
{
	return (&X)[coordinate];
}
FPN Vector4::GetLength() const
{
	return (FPN)sqrt((FPN)(X * X + Y * Y + Z * Z + W * W));
}
void Vector4::Normalize()
{
	*this /= GetLength();
}
Vector4 Vector4::GetNormalized() const
{
	return *this / GetLength();
}
Vector4 Vector4::GetRotated(const Rotation rotation) const {
	return rotation.matrix * *this;
}
void Vector4::Rotate(const Rotation rotation) {
	*this = rotation.matrix * *this;
}
std::wstring ToString(const Vector4& vector)
{
	return L"(" + std::to_wstring(vector.X) + L", " + std::to_wstring(vector.Y) + L", " + std::to_wstring(vector.Z) + L", " + std::to_wstring(vector.W) + L")";
}
FPN Vector4::AngleBetween(const Vector4 vector1, const Vector4 vector2)
{
	return std::acos(vector1.GetNormalized() * vector2.GetNormalized());
}
