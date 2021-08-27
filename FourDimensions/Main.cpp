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
    Mesh cube4D = Mesh::GetCube(Vector4(0, 3, 0, 1), Rotation{}, 1, Colorization(ColorSheme::rough, sf::Color(160, 160, 160), sf::Color(150, 150, 150)));
    world.visibles.push_back(&cube4D);
    Mesh cube4D2 = Mesh::GetCube(Vector4(0, 6, 0, 0),
        Rotation{ Vector4{ 0, 0, 0, 1 }, Vector4{ 0, 1, 0, 0 }, (FPN)constants::pi / 4 },
        2, Colorization(ColorSheme::dragedCubes, sf::Color::Black, sf::Color::White));
    world.visibles.push_back(&cube4D2);
    Mesh cubeoid4D = Mesh::GetCuboid(Vector4(0, 0, 8, 0),
        Rotation{ },
        Vector4(3, 2, 2, 2), Colorization(ColorSheme::gradual, sf::Color::Red, sf::Color::Blue));
    world.visibles.push_back(&cubeoid4D);
    Mesh pillar = Mesh::GetCuboid(Vector4(0, 8, -6, 10),
        Rotation{ Vector4{ 0, 1, 0, 0}, Vector4{ 0, 0, 1, 0 }, (FPN)-constants::pi / 2 },
        Vector4(10, 3, 3, 3), Colorization(ColorSheme::gradual, sf::Color::Black, sf::Color::White));
    world.visibles.push_back(&pillar);
    Mesh longRamp = Mesh::GetCuboid(Vector4{ 10, 0, 0, 5 },
        Rotation{ Vector4{ 1, 0, 0, 0}, Vector4{ 0, 1, 0, 0 }, (FPN)constants::pi / 4 },
        Vector4{ 3, 3, 3, 10 }, Colorization{ ColorSheme::dragedCubes, sf::Color(0, 50, 200), sf::Color::Red });
    world.visibles.push_back(&longRamp);
    RotatingMesh rotatingCuboid{
        Mesh::GetCuboid(Vector4{ -10, 0, 0, 1 }, Rotation{ }, Vector4{ 1, 5, 1, 1 }, Colorization{ ColorSheme::rough, 0xA04FB4FFu, 0x7DD801FF }),
        world.lifetimeClock, Vector4{ 1, 0, 0, 0 }, Vector4{ 0, 0, 0, 1 }, (FPN)constants::pi / 4 };
    world.visibles.push_back(&rotatingCuboid);
    world.updatables.push_back(&rotatingCuboid);
    RotatingMesh rotatingCuboid2{
        Mesh::GetCuboid(Vector4{ 0, -10, 0, 3 }, Rotation{ }, Vector4{ 1, 5, 1, 1 }, Colorization{ ColorSheme::gradual, 0xA04FB4FFu, 0x7DD801FF }),
        world.lifetimeClock, Vector4{ 1, 0, 0, 0 }, Vector4{ 0, 0, 1, 0 }, (FPN)constants::pi / 4 };
    world.visibles.push_back(&rotatingCuboid2);
    world.updatables.push_back(&rotatingCuboid2);
    std::vector<Vector4> path;
    path.push_back(Vector4(2, 3, 2, 1));
    path.push_back(Vector4(2, 3, -2, 1));
    path.push_back(Vector4(-2, 3, -2, 1));
    path.push_back(Vector4(-2, 3, 2, 1));
    MovingMesh movingMesh{ Mesh::GetCube(Vector4{ }, Rotation{ }, 1, Colorization{ sf::Color::Yellow }), world.lifetimeClock, path, 1 };
    world.visibles.push_back(&movingMesh);
    world.updatables.push_back(&movingMesh);
    std::vector<Vector4> path2;
    path2.push_back(Vector4(5, 8, -6, -2));
    path2.push_back(Vector4(5, 8, -6, 18));
    MovingMesh elevator{ Mesh::GetCube(Vector4{ }, Rotation{ }, (FPN)2, Colorization{ ColorSheme::rough, 0x000000FFu, 0x808080FF }), world.lifetimeClock, path2, 2 };
    world.visibles.push_back(&elevator);
    world.updatables.push_back(&elevator);

    world.Run();
    return 0;
}
