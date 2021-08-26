#include "pch.h"
#include "CppUnitTest.h"
#include "Linjear4D.h"
#include "Physics4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(TetrahedronTests)
	{
	public:
		TEST_METHOD(TetrahedronConstructor1)
		{
			Tetrahedron tetrahedron{ Vector4((FPN)1.2, 2, 3, 4), Vector4(4, 2, -1, 2), Vector4(0, 1, 1, -2), Vector4(-1, 1, 1, -1), Colorization(ColorSheme::dragedCubes, sf::Color::White, sf::Color::Black) };
			Comparers::AssertAlmostSame(tetrahedron.position, Vector4((FPN)1.2, 2, 3, 4));
			Comparers::AssertAlmostSame(tetrahedron.corners[0], Vector4((FPN)2.8, 0, -4, -2));
			Comparers::AssertAlmostSame(tetrahedron.corners[1], Vector4((FPN)-1.2, -1, -2, -6));
			Comparers::AssertAlmostSame(tetrahedron.corners[2], Vector4((FPN)-2.2, -1, -2, -5));
		}
		TEST_METHOD(TetrahedronConstructor2)
		{
			Vector4 corners[4] = { Vector4((FPN)1.2, 2, 3, 4), Vector4(4, 2, -1, 2), Vector4(0, 1, 1, -2), Vector4(-1, 1, 1, -1) };
			Tetrahedron tetrahedron{ corners, Colorization(ColorSheme::dragedCubes, sf::Color::White, sf::Color::Black) };
			Comparers::AssertAlmostSame(tetrahedron.position, Vector4((FPN)1.2, 2, 3, 4));
			Comparers::AssertAlmostSame(tetrahedron.corners[0], Vector4((FPN)2.8, 0, -4, -2));
			Comparers::AssertAlmostSame(tetrahedron.corners[1], Vector4((FPN)-1.2, -1, -2, -6));
			Comparers::AssertAlmostSame(tetrahedron.corners[2], Vector4((FPN)-2.2, -1, -2, -5));
		}
		TEST_METHOD(TetrahedronConstructor3)
		{
			Tetrahedron tetrahedron1{ Vector4((FPN)1.2, 2, 3, 4), Vector4(4, 2, -1, 2), Vector4(0, 1, 1, -2), Vector4(-1, 1, 1, -1), Colorization(ColorSheme::dragedCubes, sf::Color::White, sf::Color::Black) };
			Tetrahedron tetrahedron2{ tetrahedron1 };
			Comparers::AssertAlmostSame(tetrahedron1.position, tetrahedron2.position);
			Comparers::AssertAlmostSame(tetrahedron1.corners[0], tetrahedron2.corners[0]);
			Comparers::AssertAlmostSame(tetrahedron1.corners[1], tetrahedron2.corners[1]);
			Comparers::AssertAlmostSame(tetrahedron1.corners[2], tetrahedron2.corners[2]);
		}
		TEST_METHOD(TetrahedronRayCasting1)
		{
			Tetrahedron tetrahedron(Vector4(0, 0, 0, 0), Vector4(2, 0, 0, 0), Vector4(0, 2, 0, 0), Vector4(0, 0, 2, 0), Colorization(ColorSheme::simple, sf::Color::Black, sf::Color::Black));
			Comparers::AssertAlmostSame(1, tetrahedron.RayCast(Vector4((FPN)0.1, (FPN)0.1, (FPN)0.1, 1), Vector4(0, 0, 0, -2)));
		}
		TEST_METHOD(TetrahedronRayCasting2)
		{
			Tetrahedron tetrahedron(Vector4(0, 0, 0, 0), Vector4(2, 0, 0, 0), Vector4(0, 2, 0, 0), Vector4(0, 0, 2, 0), Colorization(ColorSheme::simple, sf::Color::Black, sf::Color::Black));
			Assert::IsTrue(tetrahedron.RayCast(Vector4((FPN)0.1, (FPN)0.1, (FPN)0.1, 1), Vector4(0, 0, 0, 1)) < 0);
		}
		TEST_METHOD(TetrahedronRayCastingGPUCopy1)
		{

			const Vector4 position{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 };
			const Vector4 screenCenter{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 };

			Tetrahedron tetrahedron{
				Vector4{ 2, 8, -2, 4 }, Vector4{ -2, 8, -2, 4 }, Vector4{ 2, 4, -2, 4 }, Vector4{ 2, 8, -2, 0 },
				Colorization{ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White}, true };
			FPN tetrahedronDistance = tetrahedron.RayCast(position, screenCenter);

			const unsigned screenWidth = 1200;
			const unsigned screenHeight = 1000;
			const Vector4 screenXVector{ 1, 1, 1, 1 };
			const Vector4 screenYVector{ -1, 1, -1, 1 };
			const unsigned x = screenWidth / 2;
			const unsigned y = screenHeight / 2;
			const Vector4 p = position - tetrahedron.position;

			const FPN rayDirectionX = screenCenter.X +
				((FPN)x - (FPN)screenWidth / 2) / (FPN)screenHeight * screenXVector.X +
				((FPN)y - (FPN)screenHeight / 2) / (FPN)screenHeight * screenYVector.X;
			const FPN rayDirectionY = screenCenter.Y +
				((FPN)x - (FPN)screenWidth / 2) / (FPN)screenHeight * screenXVector.Y +
				((FPN)y - (FPN)screenHeight / 2) / (FPN)screenHeight * screenYVector.Y;
			const FPN rayDirectionZ = screenCenter.Z +
				((FPN)x - (FPN)screenWidth / 2) / (FPN)screenHeight * screenXVector.Z +
				((FPN)y - (FPN)screenHeight / 2) / (FPN)screenHeight * screenYVector.Z;
			const FPN rayDirectionW = screenCenter.W +
				((FPN)x - (FPN)screenWidth / 2) / (FPN)screenHeight * screenXVector.W +
				((FPN)y - (FPN)screenHeight / 2) / (FPN)screenHeight * screenYVector.W;

			FPN matrix[4][4] = {
				{tetrahedron.corners[0].X, tetrahedron.corners[1].X, tetrahedron.corners[2].X, rayDirectionX},
				{tetrahedron.corners[0].Y, tetrahedron.corners[1].Y, tetrahedron.corners[2].Y, rayDirectionY},
				{tetrahedron.corners[0].Z, tetrahedron.corners[1].Z, tetrahedron.corners[2].Z, rayDirectionZ},
				{tetrahedron.corners[0].W, tetrahedron.corners[1].W, tetrahedron.corners[2].W, rayDirectionW} };
			FPN result[4] = { p.X, p.Y, p.Z, p.W };

			//Make it lower triangular
			for (unsigned q = 3; q >= 1; q--)
			{
				for (unsigned i = 0; i < q; i++)
				{
					FPN scalar = matrix[i][q] / matrix[q][q];
					//Subtract scalar * row q from row i
					for (unsigned p = 0; p < 4; p++)
					{
						matrix[i][p] -= scalar * matrix[q][p];
					}
					result[i] -= scalar * result[q];
				}
			}

			//Make all values in the diagonal 1
			for (unsigned i = 0; i < 4; i++)
			{
				FPN scalar = matrix[i][i];
				for (unsigned q = 0; q < 4; q++)
				{
					matrix[i][q] /= scalar;
				}
				result[i] /= scalar;
			}

			//Remove all entries below the diagonal
			for (unsigned i = 1; i < 4; i++)
			{
				for (unsigned q = 0; q < i; q++)
				{
					//Subtract clone.values[i][q] * row q from row i
					result[i] -= matrix[i][q] * result[q];
				}
			}

			FPN a1 = result[0];
			FPN a2 = result[1];
			FPN a3 = result[2];
			FPN currentDistance = -result[3] * sqrt(rayDirectionX * rayDirectionX + rayDirectionY * rayDirectionY + rayDirectionZ * rayDirectionZ + rayDirectionW * rayDirectionW);

			Comparers::AssertAlmostSame(tetrahedronDistance, currentDistance);
		}
	};
}
