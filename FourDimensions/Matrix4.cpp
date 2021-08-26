#include "Linjear4D.h"

Matrix4::Matrix4(const FPN values[4][4])
{
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned q = 0; q < 4; q++)
		{
			this->values[i][q] = values[i][q];
		}
	}
}
Matrix4::Matrix4(Vector4 column1, Vector4 column2, Vector4 column3, Vector4 column4)
{
	for (unsigned i = 0; i < 4; i++)
	{
		values[i][0] = column1[i];
		values[i][1] = column2[i];
		values[i][2] = column3[i];
		values[i][3] = column4[i];
	}
}
Matrix4::Matrix4(const FPN a00, const FPN a01, const FPN a02, const FPN a03, const FPN a10, const FPN a11, const FPN a12, const FPN a13, const FPN a20, const FPN a21, const FPN a22, const FPN a23, const FPN a30, const FPN a31, const FPN a32, const FPN a33)
{
	values[0][0] = a00;
	values[0][1] = a01;
	values[0][2] = a02;
	values[0][3] = a03;
	values[1][0] = a10;
	values[1][1] = a11;
	values[1][2] = a12;
	values[1][3] = a13;
	values[2][0] = a20;
	values[2][1] = a21;
	values[2][2] = a22;
	values[2][3] = a23;
	values[3][0] = a30;
	values[3][1] = a31;
	values[3][2] = a32;
	values[3][3] = a33;
}
Matrix4 Matrix4::operator+(const Matrix4& other) const
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int q = 0; q < 4; q++)
		{
			result.values[i][q] = values[i][q] + other.values[i][q];
		}
	}
	return result;
}
Matrix4 Matrix4::operator-(const Matrix4& other) const
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int q = 0; q < 4; q++)
		{
			result.values[i][q] = values[i][q] - other.values[i][q];
		}
	}
	return result;
}
Matrix4 operator*(const FPN scalar, const Matrix4& matrix4)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int q = 0; q < 4; i++)
		{
			result.values[i][q] = scalar * matrix4.values[i][q];
		}
	}
	return result;
}
Matrix4 operator*(const Matrix4& matrix4, const FPN scalar)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int q = 0; q < 4; q++)
		{
			result.values[i][q] = matrix4.values[i][q] * scalar;
		}
	}
	return result;
}
Matrix4 Matrix4::operator*(const Matrix4& other) const
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int q = 0; q < 4; q++)
		{
			result.values[i][q] = 0;
			for (int p = 0; p < 4; p++)
			{
				result.values[i][q] += values[i][p] * other.values[p][q];
			}
		}
	}
	return result;
}
Vector4 Matrix4::operator*(const Vector4& vector4) const
{
	Vector4 result{};
	for (int i = 0; i < 4; i++)
	{
		for (int q = 0; q < 4; q++)
		{
			result[i] += values[i][q] * vector4[q];
		}
	}
	return result;
}
Matrix4 Matrix4::Invers() const
{
	Matrix4 clone = *this;
	FPN inverse[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

	//Make it lower triangular
	for (unsigned q = 3; q >= 1; q--)
	{
		if (clone.values[q][q] == 0)
		{
			for (int i = q - 1; i >= 0; i--)
			{
				if (clone.values[i][q] != 0)
				{
					//Switches rows i and q
					for (unsigned p = 0; p < 4; p++)
					{
						FPN temp = clone.values[q][p];
						clone.values[q][p] = clone.values[i][p];
						clone.values[i][p] = temp;
						temp = inverse[q][p];
						inverse[q][p] = inverse[i][p];
						inverse[i][p] = temp;
					}
					break;
				}
			}
		}
		for (unsigned i = 0; i < q; i++)
		{
			FPN scalar = clone.values[i][q] / clone.values[q][q];
			//Subtract scalar * row q from row i
			for (unsigned p = 0; p < 4; p++)
			{
				clone.values[i][p] -= scalar * clone.values[q][p];
				inverse[i][p] -= scalar * inverse[q][p];
			}
		}
	}

	//Make all values in the diagonal 1
	for (unsigned i = 0; i < 4; i++)
	{
		FPN scalar = clone.values[i][i];
		for (unsigned q = 0; q < 4; q++)
		{
			clone.values[i][q] /= scalar;
			inverse[i][q] /= scalar;
		}
	}

	//Remove all entries below the diagonal
	for (unsigned i = 1; i < 4; i++)
	{
		for (unsigned q = 0; q < i; q++)
		{
			//Subtract clone.values[i][q] * row q from row i
			for (unsigned p = 0; p < 4; p++)
			{
				inverse[i][p] -= clone.values[i][q] * inverse[q][p];
			}
		}
	}
	return Matrix4(inverse);
}
Matrix4 Matrix4::Transpose() const
{
	FPN inside[4][4];
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned q = 0; q < 4; q++)
		{
			inside[i][q] = values[q][i];
		}
	}
	return Matrix4{ inside };
}
Matrix4 Matrix4::Identity()
{
	return Matrix4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}
FPN Matrix4::Determinant() const
{
	return
		values[0][0] * (
			values[1][1] * (values[2][2] * values[3][3] - values[2][3] * values[3][2]) -
			values[1][2] * (values[2][1] * values[3][3] - values[2][3] * values[3][1]) +
			values[1][3] * (values[2][1] * values[3][2] - values[2][2] * values[3][1])) -
		values[0][1] * (
			values[1][0] * (values[2][2] * values[3][3] - values[2][3] * values[3][2]) -
			values[1][2] * (values[2][0] * values[3][3] - values[2][3] * values[3][0]) +
			values[1][3] * (values[2][0] * values[3][2] - values[2][2] * values[3][0])) +
		values[0][2] * (
			values[1][0] * (values[2][1] * values[3][3] - values[2][3] * values[3][1]) -
			values[1][1] * (values[2][0] * values[3][3] - values[2][3] * values[3][0]) +
			values[1][3] * (values[2][0] * values[3][1] - values[2][1] * values[3][0])) -
		values[0][3] * (
			values[1][0] * (values[2][1] * values[3][2] - values[2][2] * values[3][1]) -
			values[1][1] * (values[2][0] * values[3][2] - values[2][2] * values[3][0]) +
			values[1][2] * (values[2][0] * values[3][1] - values[2][1] * values[3][0]));
}
