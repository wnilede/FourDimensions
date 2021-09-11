#include "pch.h"
#include "CppUnitTest.h"
#include <limits>
#include "Linjear4D.h"
#include "Physics4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(RayCastTests)
	{
	public:
		TEST_METHOD(RayCastCuboid)
		{
			std::vector<Visible*> visibles;
			Mesh mesh{ Mesh::GetCuboid(Vector4{ }, Rotation{ }, Vector4(2, 1, 2, 2), Colorization{ sf::Color::Blue }) };
			visibles.push_back(&mesh);
			FPN distance = RayCast(visibles, Vector4(), Vector4(1, 1, 1, 1));
			Comparers::AssertAlmostSame(2, distance);
		}
		TEST_METHOD(RayCastCuboid2)
		{
			std::vector<Visible*> visibles;
			Mesh mesh{ Mesh::GetCuboid(Vector4{ }, Rotation{ Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), (FPN)constants::pi / 4 }, Vector4(2, 1, 2, 2), Colorization{ sf::Color::Blue }) };
			visibles.push_back(&mesh);
			FPN distance = RayCast(visibles, Vector4(), Vector4(1, 1, 1, 1));
			Comparers::AssertAlmostSame(2, distance);
		}
		TEST_METHOD(RayCastCuboid3)
		{
			Mesh mesh{ Mesh::GetCuboid(Vector4{0, 0, (FPN)0.8, (FPN)0.8 }, Rotation{ Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), (FPN)constants::pi / 4 }, Vector4(1, 1, 1, 1), Colorization{ sf::Color::Blue }) };
			FPN distance = mesh.RayCast(Vector4(0, 0, 0, 0), Vector4(1, 0, 0, 0));
			Assert::AreEqual(std::numeric_limits<FPN>().infinity(), distance);
		}
		TEST_METHOD(RayCastCuboid4)
		{
			Mesh mesh{ Mesh::GetCube(Vector4{ }, Rotation{ Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), (FPN)constants::pi / 4 }, 10, Colorization{ ColorSheme::gradual, sf::Color::White, sf::Color::Black}) };
			FPN distance = mesh.RayCast(Vector4((FPN)-0.978006780, (FPN)13.3368912, (FPN)-10.7116528, (FPN)0.997174561), Vector4(0, (FPN)-0.577350259, (FPN)-0.577350259, (FPN)0.577350259));
			Assert::AreEqual(std::numeric_limits<FPN>().infinity(), distance);
		}
	};
}
