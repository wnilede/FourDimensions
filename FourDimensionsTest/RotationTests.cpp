#include "pch.h"
#include "CppUnitTest.h"
#include "Linjear4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(Rotation4Tests)
	{
	public:
		TEST_METHOD(InvalidConstruction1)
		{
			Assert::ExpectException<std::invalid_argument, void>([] { Rotation rotation{ Matrix4{ 0, 2, 3, 4, 5, 2, 3, (FPN)-2.3, -2, 3, (FPN)0.1, 2, -4, 3, 2, 0} }; });
		}
		TEST_METHOD(InvalidConstruction2)
		{
			Assert::ExpectException<std::invalid_argument, void>([] { Rotation rotation{ Matrix4{ 0, 1, 0, 0, 1 / std::sqrt((FPN)2), 0, 1 / std::sqrt((FPN)2), 0, 0, 0, 0, -1, -1 / std::sqrt((FPN)2), 0, 1 / std::sqrt((FPN)2), 0 } }; });
		}
		TEST_METHOD(Inversion)
		{
			Rotation rotation{ Matrix4{ 0, -1, 0, 0, 1 / std::sqrt((FPN)2), 0, 1 / std::sqrt((FPN)2), 0, 0, 0, 0, -1, -1 / std::sqrt((FPN)2), 0, 1 / std::sqrt((FPN)2), 0} };
			Comparers::AssertAlmostSame(Rotation{}, rotation + rotation.Invers());
		}
		TEST_METHOD(Rotatate1)
		{
			Rotation rotation{ Vector4{0, 0, 0, 1}, Vector4{0, 1, 0, 0}, (FPN)constants::pi, (FPN)constants::pi };
			Comparers::AssertAlmostSame(Matrix4::Identity() * -1, rotation.matrix);
		}
		TEST_METHOD(Rotatate2)
		{
			Rotation rotation{ Vector4{0, 0, 0, 1}, Vector4{0, 1, 0, 0}, 0, (FPN)constants::pi };
			Comparers::AssertAlmostSame(Vector4{ 2, 1, (FPN)-0.2, 7 }, Vector4{ 2, -1, (FPN)-0.2, -7 }.GetRotated(rotation));
		}
		TEST_METHOD(ConstructorAround)
		{
			Rotation rotation{ Vector4{7, 0, 0, 0}, Vector4{1, 0, 1, 0}, (FPN)constants::pi };
			Comparers::AssertAlmostSame(Vector4{ 2, 1, (FPN)-0.2, 7 }, Vector4{ 2, -1, (FPN)-0.2, -7 }.GetRotated(rotation));
		}
		TEST_METHOD(ConstructorAroundInside)
		{
			Rotation rotation{ Vector4{2, 0, 3, -1}, Vector4{2, -5, -1, -1}, (FPN)constants::pi, (FPN)constants::pi };
			Comparers::AssertAlmostSame(Vector4{ -1, 2, -2, (FPN)0.2 }, Vector4{ 1, -2, 2, (FPN)-0.2 }.GetRotated(rotation));
		}
		TEST_METHOD(Plus)
		{
			Comparers::AssertAlmostSame(Rotation{ Vector4{ 0, 2, 3, -4}, Vector4{ -1, 2, -1, 3}, 7 }, Rotation{ Vector4{ 0, 2, 3, -4}, Vector4{ -1, 2, -1, 3}, 2 } + Rotation{ Vector4{ 0, 2, 3, -4}, Vector4{ -1, 2, -1, 3}, 5 });
		}
		TEST_METHOD(Minus)
		{
			Comparers::AssertAlmostSame(Rotation{ Vector4{ 0, 2, 3, -4}, Vector4{ -1, 2, -1, 3}, 2 }, Rotation{ Vector4{ 0, 2, 3, -4}, Vector4{ -1, 2, -1, 3}, 7 } - Rotation{ Vector4{ 0, 2, 3, -4}, Vector4{ -1, 2, -1, 3}, 5 });
		}
		TEST_METHOD(MinusEqual)
		{
			Rotation rotation{ Vector4{ 0, 3, 7, 4}, Vector4{ -1, 2, 5, 3}, 3 };
			rotation -= Rotation{ Vector4{ 0, 3, 7, 4}, Vector4{ -1, 2, 5, 3}, 1 };
			Comparers::AssertAlmostSame(Rotation{ Vector4{ 0, 3, 7, 4}, Vector4{ -1, 2, 5, 3}, 2 }, rotation);
		}
	};
}
