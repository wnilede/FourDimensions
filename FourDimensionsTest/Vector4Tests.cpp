#include "pch.h"
#include "CppUnitTest.h"
#include "Linjear4D.h"
#include "Physics4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(Vector4Tests)
	{
	public:
		TEST_METHOD(Vector41)
		{
			Vector4 vector1(0, 1, 1, 2);
			Vector4 vector2(1, 2, -3, 2);
			if (Vector4(1, 3, -2, 4) != vector1 + vector2)
				Assert::Fail();
			Assert::AreEqual(Vector4(1, 3, -2, 4), vector1 + vector2);
			Assert::AreEqual(Vector4(-1, -1, 4), vector1 - vector2);
			Vector4 vector4{ 0, 2, -4 };
			vector4 /= 2;
			Assert::AreEqual(vector4, Vector4(0, 1, -2, 0));
			Vector4 vector3 = vector1;
			vector3.Normalize();
			Assert::AreEqual(vector3, vector1.GetNormalized());
			for (int i = 0; i < 4; i++)
			{
				vector3[i] = vector2[i];
			}
			Assert::AreEqual(vector2, vector3);
			Vector4 vector5{};
			for (int i = 0; i < 4; i++)
			{
				Assert::IsFalse(isfinite(vector5.GetNormalized()[i]));
			}
			vector1 -= {1, -1, 0, 3};
			Assert::AreEqual(Vector4(-1, 2, 1, -1), vector1);
		}
		TEST_METHOD(Vector42)
		{
			Vector4 vector{ 0, (FPN)-1.1, 1, 2 };
			vector += Vector4{ 7, 2, (FPN)1.1, -3 };
			Comparers::AssertAlmostSame(Vector4{ 7, (FPN)0.9, (FPN)2.1, -1 }, vector);
		}
		TEST_METHOD(AngleTest)
		{
			Comparers::AssertAlmostSame((FPN)constants::pi / 2, Vector4::AngleBetween(Vector4{ 1, 0, 2, -1 }, Vector4{ -2, (FPN)-0.9, 1, 0 }));
		}
	};
}