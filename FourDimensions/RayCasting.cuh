#pragma once

#pragma warning( push )
#pragma warning( disable : 26812)
#pragma warning( disable : 4003 )
#pragma warning( disable : 6001 )
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/sequence.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/replace.h>
#include <thrust/functional.h>
#include <thrust/zip_function.h>
#pragma warning( pop )

#include "Physics4D.h"
#include <mutex>

struct RayCasterFunctor
{
    const Vector4 screenCenter;
    const Vector4 screenXVector;
    const Vector4 screenYVector;
    const unsigned screenHeight;
    const unsigned screenWidth;
    RayCasterFunctor(const Player player, const unsigned screenHeight, const unsigned screenWidth);
};
struct RayCasterSpace3DFunctor : RayCasterFunctor
{
    const Space3D space3D;
    const FPN relativePositionTimesNorm;
    RayCasterSpace3DFunctor(const Player player, const Space3D space3D, const unsigned screenHeight, const unsigned screenWidth);
    __host__ __device__
        void operator()(const unsigned& i, FPN& distance, unsigned& color) const;
};
struct RayCasterTetrahedronFunctor : RayCasterFunctor
{
    const Tetrahedron tetrahedron;
    const Vector4 p;
    RayCasterTetrahedronFunctor(const Player player, const Tetrahedron space3D, const unsigned screenHeight, const unsigned screenWidth);
    __host__ __device__
        void operator()(const unsigned& i, FPN& distance, unsigned& color) const;
};
struct RayCaster
{
    RayCaster(sf::RenderWindow& window);
    ~RayCaster();
    //Must be set before RayCastScreen() is called.
    VisiblesImage* pVisiblesImage;
    //Should be called after pVisiblesImage is set.
    void RayCastScreen();
private:
    sf::RenderWindow& window;
    const unsigned height;
    const unsigned width;
    sf::Uint8* const pixels;
    sf::Texture texture;
    sf::Sprite sprite;
    thrust::device_vector<FPN> distance;
    thrust::device_vector<unsigned> color;
    const thrust::counting_iterator<unsigned> pixelIndex{ 0 };
};
