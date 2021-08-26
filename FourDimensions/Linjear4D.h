#pragma once

#ifndef FPN
#define FPN float
#endif

#include <vector>
#include <string>

struct Rotation;

struct Vector4
{
	FPN X;
	FPN Y;
	FPN Z;
	FPN W;
	Vector4() = default;
	Vector4(const FPN x, const FPN y, const FPN z);
	Vector4(const FPN x, const FPN y, const FPN z, const FPN w);
	bool operator==(const Vector4 other) const;
	bool operator!=(const Vector4 other) const;
	Vector4 operator+(const Vector4 other) const;
	void operator+=(const Vector4 other);
	Vector4 operator-(const Vector4 other) const;
	void operator-=(const Vector4 other);
	Vector4 operator-() const;
	FPN operator*(const Vector4 other) const;
	FPN& operator[](const int coordinate);
	FPN operator[](const int coordinate) const;
	FPN GetLength() const;
	Vector4 GetNormalized() const;
	void Normalize();
	Vector4 GetRotated(const Rotation rotation) const;
	void Rotate(const Rotation rotation);
	static FPN AngleBetween(const Vector4 vector1, const Vector4 vector2);
};
Vector4 operator*(const FPN scalar, const Vector4 vector);
Vector4 operator*(const Vector4 vector, const FPN scalar);
void operator*=(Vector4& vector, const FPN scalar);
Vector4 operator/(Vector4 vector, FPN scalar);
void operator/=(Vector4& vector, FPN scalar);
std::wstring ToString(const Vector4& vector);

struct Matrix4
{
	FPN values[4][4];
	Matrix4() = default;
	Matrix4(const FPN values[4][4]);
	Matrix4(Vector4 column1, Vector4 column2, Vector4 column3, Vector4 column4);
	Matrix4(const FPN a00, const FPN a01, const FPN a02, const FPN a03, const FPN a10, const FPN a11, const FPN a12, const FPN a13, const FPN a20, const FPN a21, const FPN a22, const FPN a23, const FPN a30, const FPN a31, const FPN a32, const FPN a33);
	Matrix4 operator+(const Matrix4& other) const;
	Matrix4 operator-(const Matrix4& other) const;
	Matrix4 operator*(const Matrix4& other) const;
	Vector4 operator*(const Vector4& vector4) const;
	Matrix4 Invers() const;
	Matrix4 Transpose() const;
	static Matrix4 Identity();
	FPN Determinant() const;
};
Matrix4 operator*(const FPN scalar, const Matrix4& matrix4);
Matrix4 operator*(const Matrix4& matrix4, const FPN scalar);

struct Rotation
{
	Matrix4 matrix;
	Rotation();
	Rotation::Rotation(Matrix4 matrix, bool checkValidity = true);
	//Rotates around the plane spanned by vector1 and vector2 by angleAround and inside it by angleInside.
	Rotation(Vector4 vector1, Vector4 vector2, FPN angleAround, FPN angleInside);
	//Rotates around the plane spanned by vector1 and vector2 by angleAround.
	Rotation(Vector4 vector1, Vector4 vector2, FPN angleAround);
	Rotation operator+(const Rotation other) const;
	void operator+=(const Rotation other);
	Rotation operator-() const;
	Rotation operator-(const Rotation other) const;
	void operator-=(const Rotation other);
	Rotation Invers() const;
};
