#include "World.h"

JumpTutorialWorld::JumpTutorialWorld()
{
    player.position = Vector4(1, 1, 1, 1);
    visibles.push_back(&surroundingCube);
    visibles.push_back(&platform1);
    visibles.push_back(&platform2);
    visibles.push_back(&platform3);
    visibles.push_back(&platform4);
    visibles.push_back(&platform5);
    visibles.push_back(&platform6);
    visibles.push_back(&platform7);
    SetGoalPosition(Vector4(6, 6, 6, 14));
}
