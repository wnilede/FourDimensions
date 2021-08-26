#include "pch.h"
#include "CppUnitTest.h"
#include "Physics4D.h"
#include "Comparers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFourDimensions
{
	TEST_CLASS(VisiblesImageTests)
	{
	public:
		//TEST_METHOD(NewVisiblesAreEqual)
		//{
		//	Space3D space{ Vector4(1, 2, 3, 4), Vector4(5, 6, 7, 8), sf::Color::Red };
		//	Tetrahedron tetrahedron{
		//		Vector4((FPN)1.2, 2, 3, 4), Vector4(4, 2, -1, 2), Vector4(0, 1, 1, -2), Vector4(-1, 1, 1, -1),
		//		Colorization(ColorSheme::dragedCubes, sf::Color::White, sf::Color::Black) };
		//	std::vector<Visible*> visibles{};
		//	visibles.push_back(&space);
		//	visibles.push_back(&tetrahedron);
		//	VisiblesImage visiblesImage{ visibles, std::mutex{} };
		//	Comparers::AssertAlmostSame(space.position, static_cast<Space3D*>(visiblesImage.visibles[0])->position);
		//	Comparers::AssertAlmostSame(space.norm, static_cast<Space3D*>(visiblesImage.visibles[0])->norm);
		//	Assert::AreEqual(space.color, static_cast<Space3D*>(visiblesImage.visibles[0])->color);
		//	Assert::AreEqual(space.id, static_cast<Space3D*>(visiblesImage.visibles[0])->id);
		//	Assert::AreEqual(tetrahedron.corners[2], static_cast<Tetrahedron*>(visiblesImage.visibles[1])->corners[2]);
		//	Assert::AreEqual(tetrahedron.actuallyParallelepiped, static_cast<Tetrahedron*>(visiblesImage.visibles[1])->actuallyParallelepiped);
		//}
		//TEST_METHOD(NewVisiblesAreNotSame)
		//{
		//	Space3D space{ Vector4(5, 6, 7, 8), Vector4(1, 2, 3, 4), sf::Color::Red };
		//	Tetrahedron tetrahedron{
		//		Vector4((FPN)1.2, 2, 3, 4), Vector4(4, 2, -1, 2), Vector4(0, 1, 1, -2), Vector4(-1, 1, 1, -1),
		//		Colorization(ColorSheme::dragedCubes, sf::Color::White, sf::Color::Black) };
		//	std::vector<Visible*> visibles{};
		//	visibles.push_back(&space);
		//	visibles.push_back(&tetrahedron);
		//	VisiblesImage visiblesImage{ visibles, std::mutex{} };
		//	space.norm = Vector4{ 3, 4, 2, 1 };
		//	space.position.Y = -3;
		//	Comparers::AssertAlmostSame(Vector4{ 1, 2, 3, 4 }, static_cast<Space3D*>(visiblesImage.visibles[0])->position);
		//	Comparers::AssertAlmostSame(Vector4(5, 6, 7, 8).GetNormalized(), static_cast<Space3D*>(visiblesImage.visibles[0])->norm);
		//	Tetrahedron tetrahedron2{
		//		Vector4((FPN)1.1, -2, 4, 3), Vector4(1, -2, 1, 3), Vector4(2, 0, 0, -2), Vector4(-1, 2, -1, 0),
		//		Colorization(ColorSheme::simple, sf::Color::Black, sf::Color::Yellow) };
		//	visibles[1] = &tetrahedron2;
		//	Comparers::AssertAlmostSame(Vector4((FPN)1.2, 2, 3, 4), static_cast<Tetrahedron*>(visiblesImage.visibles[1])->position);
		//	Assert::AreEqual(false, static_cast<Tetrahedron*>(visiblesImage.visibles[1])->actuallyParallelepiped);
		//	visibles.push_back(&tetrahedron);
		//	Assert::AreEqual((size_t)2, visiblesImage.visibles.size());
		//}
	};
}
