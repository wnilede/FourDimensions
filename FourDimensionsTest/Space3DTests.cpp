#include "pch.h"
#include "CppUnitTest.h"
#include "Linjear4D.h"
#include "Physics4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(Space3DTests)
	{
	public:
		TEST_METHOD(Space3DTest1)
		{
			Space3D space(Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), sf::Color::Black);
			Comparers::AssertAlmostSame(0, space.RayCast(Vector4(0, 0, 1), Vector4(0, 0, -1)));
		}
		TEST_METHOD(Space3DTest2)
		{
			Space3D space(Vector4(0, 1, 0, 0), Vector4(0, 1, 0, 0), sf::Color::Black);
			Comparers::AssertAlmostSame(2, space.RayCast(Vector4(1, -1, -3), Vector4(0, 1, 0)));
		}
		TEST_METHOD(Space3DTest3)
		{
			Space3D space(Vector4(0, -1, -1, 0), Vector4(0, -1, 0, 0), sf::Color::Black);
			Assert::IsTrue(space.RayCast(Vector4(1, 2, 3, 7), Vector4(0, 1, 0)) < 0);
		}
		TEST_METHOD(Space3DTest4)
		{
			Space3D space{ Vector4(0, 0, 1, 0), Vector4(0, 0, 1, 0), sf::Color::Red };
			FPN spaceDistance = space.RayCast(
				Vector4{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 },
				Vector4{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 });
			Tetrahedron parallelepiped{
				Vector4{ -100, -100, 1, -100 }, Vector4{ 100, -100, 1, -100 }, Vector4{ -100, 100, 1, -100 }, Vector4{ -100, -100, 1, 100 },
				Colorization{ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White}, true };
			FPN parallelepipedDistance = parallelepiped.RayCast(
				Vector4{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 },
				Vector4{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 });
			Comparers::AssertAlmostSame(parallelepipedDistance, spaceDistance);
		}
		TEST_METHOD(Space3DTest5)
		{
			Space3D space{ Vector4(0, 0, 1, 0), Vector4(0, 0, 1, 0), sf::Color::Red };
			FPN spaceDistance = space.RayCast(
				Vector4{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 },
				Vector4{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 });
			Tetrahedron parallelepiped{
				Vector4{ -100, -100, 0, -100 }, Vector4{ 100, -100, 0, -100 }, Vector4{ -100, 100, 0, -100 }, Vector4{ -100, -100, 0, 100 },
				Colorization{ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White}, true };
			FPN parallelepipedDistance = parallelepiped.RayCast(
				Vector4{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 },
				Vector4{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 });
			Assert::IsTrue(parallelepipedDistance > 0 && parallelepipedDistance < spaceDistance);
		}
		TEST_METHOD(Space3DTest6)
		{
			Space3D space{ Vector4(0, 0, 1, 0), Vector4(0, 0, 1, 0), sf::Color::Red };
			FPN spaceDistance = space.RayCast(
				Vector4{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 },
				Vector4{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 });
			Tetrahedron parallelepiped{
				Vector4{ 2, 8, -2, 4 }, Vector4{ -2, 8, -2, 4 }, Vector4{ 2, 4, -2, 4 }, Vector4{ 2, 8, -2, 0 },
				Colorization{ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White}, true };
			FPN parallelepipedDistance = parallelepiped.RayCast(
				Vector4{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 },
				Vector4{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 });
			Assert::IsTrue(parallelepipedDistance > 0 && parallelepipedDistance < spaceDistance);
		}
		TEST_METHOD(Space3DTest7)
		{
			const Vector4 position{ (FPN)0.609922, (FPN)5.498534, (FPN)-5.969094, (FPN)1.700000 };
			const Vector4 screenCenter{ (FPN)-0.057880, (FPN)0.048593, (FPN)0.484171, (FPN)-0.099335 };

			Space3D space{ Vector4(0, 0, 1, 0), Vector4(0, 0, 1, 0), sf::Color::Red };
			FPN spaceDistance = space.RayCast(position, screenCenter);

			const unsigned screenWidth = 1200;
			const unsigned screenHeight = 1000;
			const Vector4 screenXVector{ 1, 1, 1, 1 };
			const Vector4 screenYVector{ -1, 1, -1, 1 };
			const unsigned x = screenWidth / 2;
			const unsigned y = screenHeight / 2;

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

			const FPN currentDistance = (space.position - position) * space.norm /
				(rayDirectionX * space.norm.X + rayDirectionY * space.norm.Y + rayDirectionZ * space.norm.Z + rayDirectionW * space.norm.W) *
				sqrt(rayDirectionX * rayDirectionX + rayDirectionY * rayDirectionY + rayDirectionZ * rayDirectionZ + rayDirectionW * rayDirectionW);

			Comparers::AssertAlmostSame(spaceDistance, currentDistance);
		}
	};
}