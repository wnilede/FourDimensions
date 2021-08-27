#include "World.h"

TestingWorld::TestingWorld()
{
    player.position = Vector4(0, 0, 0, 1);
    visibles.push_back(&ground);
    //world.visibles.push_back(&wall1);
    //world.visibles.push_back(&wall2);
    //world.visibles.push_back(&tetrahedron1);
    //world.visibles.push_back(&tetrahedron2);
    //world.visibles.push_back(&tetrahedron3);
    //world.visibles.push_back(&parallelepepid);
    visibles.push_back(&cube4D);
    visibles.push_back(&cube4D2);
    visibles.push_back(&cubeoid4D);
    visibles.push_back(&pillar);
    visibles.push_back(&longRamp);
    visibles.push_back(&rotatingCuboid);
    updatables.push_back(&rotatingCuboid);
    visibles.push_back(&rotatingCuboid2);
    updatables.push_back(&rotatingCuboid2);
    std::vector<Vector4> path;
    path.push_back(Vector4(2, 3, 2, 1));
    path.push_back(Vector4(2, 3, -2, 1));
    path.push_back(Vector4(-2, 3, -2, 1));
    path.push_back(Vector4(-2, 3, 2, 1));
    movingMesh.SetPathCorners(path);
    visibles.push_back(&movingMesh);
    updatables.push_back(&movingMesh);
    std::vector<Vector4> path2;
    path2.push_back(Vector4(5, 8, -6, -2));
    path2.push_back(Vector4(5, 8, -6, 18));
    elevator.SetPathCorners(path2);
    visibles.push_back(&elevator);
    updatables.push_back(&elevator);
    SetGoalPosition(Vector4(10, 10, 10, 1));
}
