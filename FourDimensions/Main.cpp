#include <iostream>
#include <limits>
#include <cmath>
#include <execution>
#include <algorithm>

#include "Linjear4D.h"
#include "Physics4D.h"
#include "RayCasting.cuh"
#include "World.h"

int main()
{
    World world{};

    world.player.position = Vector4(0, 0, 0, 1);
    Space3D ground{ Vector4(0, 0, 0, 1), Vector4(), sf::Color::Green };
    world.visibles.push_back(&ground);
    Space3D wall1{ Vector4(0, 0, 1, 0), Vector4(0, 0, 1, 0), sf::Color::Red };
    //world.visibles.push_back(&wall1);
    Space3D wall2{ Vector4(0, 1, 0, 0), Vector4(0, 1, 0, 0), sf::Color::Yellow };
    //world.visibles.push_back(&wall2);
    Tetrahedron tetrahedron1{ Vector4(-1, 1, 1, 1), Vector4(1, 1, -1, 1), Vector4(1, 1, 1, -1), Vector4(-1, 1, -1, -1), Colorization(ColorSheme::simple, sf::Color::Black, sf::Color::Black) };
    //world.visibles.push_back(&tetrahedron1);
    Tetrahedron tetrahedron2{ Vector4(2, 11, 1, 1), Vector4(1, 11, 2, 1), Vector4(1, 11, 1, -1), Vector4(-1, 11, 2, -1), Colorization(ColorSheme::simple, sf::Color::White, sf::Color::White) };
    //world.visibles.push_back(&tetrahedron2);
    Tetrahedron tetrahedron3{ Vector4(-1, 1, 11, 1), Vector4(1, 1, 9, 1), Vector4(1, 1, 11, -1), Vector4(-1, 1, 9, -1), Colorization(ColorSheme::simple, sf::Color::Magenta, sf::Color::Magenta) };
    //world.visibles.push_back(&tetrahedron3);
    Tetrahedron parallelepepid{ Vector4(2, 0, 0, 2), Vector4(3, 0, 0, 2), Vector4(2, 1, 0, 2), Vector4(2, 0, 1, 2), Colorization(ColorSheme::simple, sf::Color::Cyan, sf::Color::Cyan), true };
    //world.visibles.push_back(&parallelepepid);
    Cube4D cube4D{ Vector4(0, 3, 0, 1), Rotation{}, 1, Colorization(ColorSheme::dragedCubes, sf::Color(100, 100, 100), sf::Color(150, 150, 150)) };
    world.visibles.push_back(&cube4D);
    Cube4D cube4D2{ Vector4(0, 6, 0, 0),
        Rotation{ Vector4{ 0, 0, 0, 1}, Vector4{ 0, 1, 0, 0 }, (FPN)constants::pi / 4 },
        2, Colorization(ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White) };
    world.visibles.push_back(&cube4D2);
    Cuboid4D cubeoid4D{ Vector4(0, 0, 8, 0),
        Rotation{ },
        Vector4(3, 2, 2, 2), Colorization(ColorSheme::gradual, sf::Color::Red, sf::Color::Blue) };
    world.visibles.push_back(&cubeoid4D);
    Cuboid4D pillar{ Vector4(0, 8, -6, 10),
        Rotation{ Vector4{ 0, 1, 0, 0}, Vector4{ 0, 0, 1, 0 }, (FPN)-constants::pi / 2 },
        Vector4(10, 3, 3, 3), Colorization(ColorSheme::gradual, sf::Color::Black, sf::Color::White) };
    world.visibles.push_back(&pillar);
    Cuboid4D longRamp{ Vector4{ 10, 0, 0, 5 },
        Rotation{ Vector4{ 1, 0, 0, 0}, Vector4{ 0, 1, 0, 0 }, (FPN)constants::pi / 4 },
        Vector4{ 3, 3, 3, 10 }, Colorization{ ColorSheme::dragedCubes, sf::Color(0, 50, 200), sf::Color::Red } };
    world.visibles.push_back(&longRamp);


    //for (Tetrahedron& tetrahedron : pillar.tetrahedrons) {
    //    for (Vector4& corner : tetrahedron.corners) {
    //        if (corner.GetLength() > 100) {
    //            std::cout << "Problem" << std::endl;
    //        }
    //        if (corner.X < 0.1) {
    //            corner.X = 0;
    //        }
    //        if (corner.Y < 0.1) {
    //            corner.Y = 0;
    //        }
    //        if (corner.Z < 0.1) {
    //            corner.Z = 0;
    //        }
    //        if (corner.W < 0.1) {
    //            corner.W = 0;
    //        }
    //    }
    //}


    world.Run();
    return 0;
}
