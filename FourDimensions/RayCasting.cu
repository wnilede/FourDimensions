#include "RayCasting.cuh"
#include <execution>
#include <algorithm>
#include <thread>
#include <iostream>

#pragma region RayCaster
RayCaster::RayCaster(sf::RenderWindow& window, const Player& player, const std::vector<Visible*>& visibles, std::mutex& visiblesMutex, const unsigned height, const unsigned width, std::atomic<bool>& lockTest) :
	window(window), player(player), visibles(visibles), height(height), width(width), pixels(new sf::Uint8[width * height * 4]),
	distance(height * width), color(height * width), pVisiblesImage(nullptr)
{
	texture.create(width, height);
	sprite = sf::Sprite(texture);
}
RayCaster::~RayCaster()
{
	delete[] pixels;
}
void RayCaster::RayCastScreen()
{
	thrust::fill(distance.begin(), distance.end(), std::numeric_limits<FPN>::infinity());
	thrust::fill(color.begin(), color.end(), sf::Color::Blue.toInteger());
	for (Visible* visible : pVisiblesImage->visibles)
	{
		switch (visible->id)
		{
		case Space3D::id:
			thrust::for_each(
				thrust::make_zip_iterator(pixelIndex, distance.begin(), color.begin()),
				thrust::make_zip_iterator(pixelIndex + height * width, distance.end(), color.end()),
				thrust::make_zip_function(RayCasterSpace3DFunctor(pVisiblesImage->player, *static_cast<const Space3D*>(visible), height, width)));
			break;
		case Tetrahedron::id:
			thrust::for_each(
				thrust::make_zip_iterator(pixelIndex, distance.begin(), color.begin()),
				thrust::make_zip_iterator(pixelIndex + height * width, distance.end(), color.end()),
				thrust::make_zip_function(RayCasterTetrahedronFunctor(pVisiblesImage->player, *static_cast<const Tetrahedron*>(visible), height, width)));
			break;
		case Mesh::id:
			for (Tetrahedron tetrahedron : static_cast<const Mesh*>(visible)->absoluteTetrahedrons)
			{
				thrust::for_each(
					thrust::make_zip_iterator(pixelIndex, distance.begin(), color.begin()),
					thrust::make_zip_iterator(pixelIndex + height * width, distance.end(), color.end()),
					thrust::make_zip_function(RayCasterTetrahedronFunctor(pVisiblesImage->player, tetrahedron, height, width)));
			}
			break;
		}
	}
	thrust::host_vector<unsigned> hostColor = color;
	struct VertexArrayFiller
	{
		sf::Uint8* pixels;
		thrust::host_vector<unsigned>& colors;
		const unsigned shunkSize;
		VertexArrayFiller(sf::Uint8* const pixels, thrust::host_vector<unsigned>& colors, unsigned shunkSize) :
			pixels(pixels), colors(colors), shunkSize(shunkSize)
		{ }
		void operator()(unsigned i)
		{
			for (unsigned q = i * shunkSize; q < (i + 1) * shunkSize; q++)
			{
				sf::Color color(colors[q]);
				pixels[q * 4] = color.r;
				pixels[q * 4 + 1] = color.g;
				pixels[q * 4 + 2] = color.b;
				pixels[q * 4 + 3] = color.a;
			}
		}
	};
	std::vector<std::thread> threads;
	for (unsigned i = 0; i < 10; i++) {
		threads.push_back(std::thread(VertexArrayFiller{ pixels, hostColor, width * height / 10 }, i));
	}
	for (std::thread& thread : threads) {
		thread.join();
	}
	texture.update(pixels); //Very slow, but do not know how to make faster
	window.draw(sprite);
}
FPN RayCaster::RayCastCPU(Vector4 rayOrigin, Vector4 RayDirection)
{
	FPN closestDistance = std::numeric_limits<FPN>::infinity();
	for (Visible* visible : visibles)
	{
		FPN distance;
		switch (visible->id)
		{
		case Space3D::id:
			distance = static_cast<Space3D*>(visible)->RayCast(rayOrigin, RayDirection);
			break;
		case Tetrahedron::id:
			distance = static_cast<Tetrahedron*>(visible)->RayCast(rayOrigin, RayDirection);
			break;
		case Mesh::id:
			distance = static_cast<Mesh*>(visible)->RayCast(rayOrigin, RayDirection);
			break;
		}
		if (distance >= 0 && distance < closestDistance)
			closestDistance = distance;
	}
	return closestDistance;
}
#pragma endregion

#pragma region RayCasterFunctors
RayCasterFunctor::RayCasterFunctor(const Player player, const unsigned screenHeight, const unsigned screenWidth) :
	screenCenter(player.GetScreenCenter()), screenXVector(player.GetScreenXVector()), screenYVector(player.GetScreenYVector()),
	screenHeight(screenHeight), screenWidth(screenWidth)
{ }

#pragma region RayCasterSpace3DFunctor
RayCasterSpace3DFunctor::RayCasterSpace3DFunctor(
	const Player player, const Space3D space3D, const unsigned screenHeight, const unsigned screenWidth) :
	RayCasterFunctor(player, screenHeight, screenWidth), space3D(space3D), relativePositionTimesNorm((space3D.position - player.position) * space3D.norm)
{ }
void RayCasterSpace3DFunctor::operator()(const unsigned& i, FPN& distance, unsigned& color) const
{
	const unsigned x = i % screenWidth;
	const unsigned y = i / screenWidth;
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

	const FPN currentDistance = relativePositionTimesNorm /
		(rayDirectionX * space3D.norm.X + rayDirectionY * space3D.norm.Y + rayDirectionZ * space3D.norm.Z + rayDirectionW * space3D.norm.W) *
		sqrt(rayDirectionX * rayDirectionX + rayDirectionY * rayDirectionY + rayDirectionZ * rayDirectionZ + rayDirectionW * rayDirectionW);
	if (currentDistance >= 0 && currentDistance < distance)
	{
		distance = currentDistance;
		color = space3D.color;
	}
}
#pragma endregion

#pragma region RayCasterTetrahedronFunctor
RayCasterTetrahedronFunctor::RayCasterTetrahedronFunctor(
	const Player player, const Tetrahedron tetrahedron, const unsigned screenHeight, const unsigned screenWidth) :
	RayCasterFunctor(player, screenHeight, screenWidth), tetrahedron(tetrahedron), p(player.position - tetrahedron.position)
{ }
//i is index converted to x and y. distance is shortet positive distance so far, and is replaced if better is found. color is
//best color so far, and is replaced by the color of this object if the distance to this is smaller. Strange things made to
//make it able to run on the GPU and fast.
void RayCasterTetrahedronFunctor::operator()(const unsigned& i, FPN& distance, unsigned& color) const
{
	const unsigned x = i % screenWidth;
	const unsigned y = i / screenWidth;
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

	//const FPN a1 =
	//	p[0] * (c[][] * c[][] * v + c[][] * v * c[][] + v * c[][] * c[][]) +
	//	p[1] * (c[][] * c[][] * c[][] + c[][] * c[][] * c[][] + c[][] * c[][] * c[][]) +
	//	p[2] * (c[][] * c[][] * c[][] + c[][] * c[][] * c[][] + c[][] * c[][] * c[][]) +
	//	p[3] * (c[][] * c[][] * c[][] + c[][] * c[][] * c[][] + c[][] * c[][] * c[][]);
	//const FPN t = -(
	//	p[0] * (
	//		c[0][1] * c[1][2] * c[2][3] +
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][]) +
	//	p[1] * (
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][]) +
	//	p[2] * (
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][]) +
	//	p[3] * (
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][] +
	//		c[][] * c[][] * c[][]));

	FPN matrix[4][4] = {
		{tetrahedron.corners[0].X, tetrahedron.corners[1].X, tetrahedron.corners[2].X, rayDirectionX},
		{tetrahedron.corners[0].Y, tetrahedron.corners[1].Y, tetrahedron.corners[2].Y, rayDirectionY},
		{tetrahedron.corners[0].Z, tetrahedron.corners[1].Z, tetrahedron.corners[2].Z, rayDirectionZ},
		{tetrahedron.corners[0].W, tetrahedron.corners[1].W, tetrahedron.corners[2].W, rayDirectionW} };
	FPN result[4] = { p.X, p.Y, p.Z, p.W };

	//Make it lower triangular
	for (unsigned q = 3; q >= 1; q--)
	{
		if (matrix[q][q] < (FPN)1e-3 && matrix[q][q] > (FPN)-1e-3)
		{
			for (int i = q - 1; i >= 0; i--) //Should perhaps go throgh all potential rows to switch with and take best, instead of giving up if none good enough is found.
			{
				if (matrix[i][q] >= (FPN)1e-3 || matrix[i][q] <= (FPN)-1e-3)
				{
					//Switches rows i and q
					for (unsigned p = 0; p < 4; p++)
					{
						FPN temp1 = matrix[q][p];
						matrix[q][p] = matrix[i][p];
						matrix[i][p] = temp1;
					}
					FPN temp2 = result[q];
					result[q] = result[i];
					result[i] = temp2;
					break;
				}
			}
		}

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
	FPN currentDistance = -result[3] *
		sqrt(rayDirectionX * rayDirectionX + rayDirectionY * rayDirectionY + rayDirectionZ * rayDirectionZ + rayDirectionW * rayDirectionW);

	if (currentDistance >= 0 && currentDistance < distance &&
		a1 >= 0 && a2 >= 0 && a3 >= 0 && (
		!tetrahedron.actuallyParallelepiped && a1 + a2 + a3 <= 1 ||
		tetrahedron.actuallyParallelepiped && a1 <= 1 && a2 <= 1 && a3 <= 1))
	{
		distance = currentDistance;
		switch (tetrahedron.colorization.colorScheme)
		{
		case ColorSheme::simple:
			color = tetrahedron.colorization.color1;
			break;
		case ColorSheme::dragedCubes:
			if (a1 < (FPN)0.5 != a2 < (FPN)0.5 != a3 < (FPN)0.5)
				color = tetrahedron.colorization.color1;
			else
				color = tetrahedron.colorization.color2;
			break;
		case ColorSheme::gradual:
		{
			//color = (unsigned)(a1 * (FPN)tetrahedron.colorization.color1 + (1 - a1) * (FPN)tetrahedron.colorization.color2); //Essentially what we want but becomes strange with float and unsigned and different channels owerflowing
			unsigned color1Part = (unsigned)(a1 * 0x01000000u);
			unsigned color2Part = 0x01000000u - color1Part;
			color = 0x000000FFu |
				(color1Part * (tetrahedron.colorization.color1 >> 8 & 0x000000FFu) + color2Part * (tetrahedron.colorization.color2 >> 8 & 0x000000FFu) >> 16 & 0x0000FF00u) +
				(color1Part * (tetrahedron.colorization.color1 >> 16 & 0x000000FFu) + color2Part * (tetrahedron.colorization.color2 >> 16 & 0x000000FFu) >> 8 & 0x00FF0000u) +
				(color1Part * (tetrahedron.colorization.color1 >> 24 & 0x000000FFu) + color2Part * (tetrahedron.colorization.color2 >> 24 & 0x000000FFu) & 0xFF000000u);
		}
			break;
		default:
			color = 0xFFB6C1FF;
			break;
		}
	}
}
#pragma endregion
#pragma endregion
