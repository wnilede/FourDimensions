#include "pch.h"
#include "Comparers.h"

namespace Comparers
{
	void AssertAlmostSame(const FPN expected, const FPN actual)
	{
		Assert::IsTrue(abs((double)(expected - actual)) < 0.1,
			(L"Expected value " + std::to_wstring(expected) + L" with actual value " + std::to_wstring(actual)).c_str());
	}
	void AssertAlmostSame(const Vector4 expected, const Vector4 actual)
	{
		for (unsigned i = 0; i < 4; i++)
		{
			Assert::IsTrue(abs((double)(expected[i] - actual[i])) < 0.1,
				(L"Expected value " + std::to_wstring(expected[i]) + L" with actual value " + std::to_wstring(actual[i]) +
					L" at position " + std::to_wstring(i) + L".").c_str());
		}
	}
	void AssertAlmostSame(const Matrix4 expected, const Matrix4 actual)
	{
		for (unsigned i = 0; i < 4; i++)
		{
			for (unsigned q = 0; q < 4; q++)
			{
				Assert::IsTrue(abs((double)(expected.values[i][q] - actual.values[i][q])) < 0.1,
					(L"Expected value " + std::to_wstring(expected.values[i][q]) + L" with actual value " + std::to_wstring(actual.values[i][q]) +
						L" at row " + std::to_wstring(i) + L" and column " + std::to_wstring(q) + L".").c_str());
			}
		}
	}
	void AssertAlmostSame(const Rotation expected, const Rotation actual)
	{
		AssertAlmostSame(expected.matrix, actual.matrix);
	}
}