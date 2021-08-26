#pragma once

#include "pch.h"
#include "CppUnitTest.h"
#include "Linjear4D.h"
#include "Physics4D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Comparers
{
	void AssertAlmostSame(const FPN expected, const FPN actual);
	void AssertAlmostSame(const Vector4 expected, const Vector4 actual);
	void AssertAlmostSame(const Matrix4 expected, const Matrix4 actual);
	void AssertAlmostSame(const Rotation expected, const Rotation actual);
}