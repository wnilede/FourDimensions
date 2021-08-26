#include "pch.h"
#include "CppUnitTest.h"
#include "Linjear4D.h"
#include "Physics4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(Matrix4Tests)
	{
	public:
		TEST_METHOD(Matrix4Inversion1)
		{
			FPN inside[4][4] = { {1, 2, 3, 4}, {0, -2, (FPN)0.8, 8}, {-7, (FPN)-4.7, 3, 2}, {-1, (FPN)-6.1, 2, 13} };
			Matrix4 matrix(inside);
			FPN insideId[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			Comparers::AssertAlmostSame(Matrix4(insideId), matrix.Invers() * matrix);
		}
		TEST_METHOD(Matrix4Inversion2)
		{
			FPN insideId[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			Comparers::AssertAlmostSame(Matrix4(insideId), Matrix4(insideId).Invers());
		}
		TEST_METHOD(Matrix4Inversion4)
		{
			FPN inside[4][4] = { {7, 3, 1, 5}, {1, 6, 2, 8}, {4, 5, 9, 1}, {1, 3, 3, 6} };
			Matrix4 matrix(inside);
			FPN insideId[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			Comparers::AssertAlmostSame(Matrix4(insideId), matrix.Invers() * matrix);
		}
		TEST_METHOD(Matrix4Inversion5)
		{
			FPN inside[4][4] = { {2, 0, 0, 0}, {4, (FPN)0.1, 0, 0}, {(FPN)2.2, 8, 2, 0}, {2, 1, 1, 3} };
			Matrix4 matrix(inside);
			FPN insideId[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			Comparers::AssertAlmostSame(Matrix4(insideId), matrix.Invers() * matrix);
		}
		TEST_METHOD(Matrix4Inversion6)
		{
			FPN inside[4][4] = { {0, 5, 3, 2}, {-7, 0, (FPN)2.1, 1}, {5, 8, 0, 0}, {2, 1, 1, 0} };
			Matrix4 matrix(inside);
			FPN insideId[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			Comparers::AssertAlmostSame(Matrix4(insideId), matrix.Invers() * matrix);
		}
		TEST_METHOD(Matrix4Multipilcation1)
		{
			FPN insideLeft[4][4] = { {1, 2, 0, 2}, {-1, -2, 3, (FPN)0.1}, {2, 1, 1, 0}, {0, 0, 1, -2} };
			FPN insideRight[4][4] = { {1, 1, 1, 1}, {1, 2, 3, -2}, {1, 2, -3, -1}, {-1, 0, 0, 2} };
			FPN insideResult[4][4] = { {1, 5, 7, 1}, {(FPN)-0.1, 1, -16, (FPN)0.2}, {4, 6, 2, -1}, {3, 2, -3, -5} };
			Comparers::AssertAlmostSame(Matrix4(insideResult), Matrix4(insideLeft) * Matrix4(insideRight));
		}
		TEST_METHOD(Matrix4Vector4Multiplication1)
		{
			Vector4 vector{ 1, 0, (FPN)-2.5, -6 };
			FPN inside[4][4] = { {2, 0, 0, 0}, {4, (FPN)0.1, 0, 0}, {(FPN)-2.2, 8, -2, 0}, {2, 1, 1, 3} };
			Matrix4 matrix(inside);
			Comparers::AssertAlmostSame(Vector4{ 2, 4, (FPN)2.8, (FPN)-18.5 }, matrix * vector);
		}
		TEST_METHOD(Matrix4Constructor1)
		{
			FPN inside[4][4] = { {1, 2, 0, 2}, {-1, -2, 3, (FPN)0.1}, {2, 1, 1, 0}, {0, 0, 1, -2} };
			Comparers::AssertAlmostSame(Matrix4{ inside }, Matrix4{ Vector4(1, -1, 2, 0), Vector4(2, -2, 1, 0), Vector4(0, 3, 1, 1), Vector4(2, (FPN)0.1, 0, -2) });
		}
		TEST_METHOD(Matrix4Determinant1)
		{
			Matrix4 matrix{ 3, 8, 9, 5, (FPN)-8.1, -3, (FPN)-2.7, 2, -4, 0, 2, (FPN)0.1, -6, 2, 5, -1 };
			Comparers::AssertAlmostSame((FPN)-291.7, matrix.Determinant());
		}
	};
}
