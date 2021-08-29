#include "World.h"

JumpTutorialWorld::JumpTutorialWorld()
{
    player.position = Vector4(1, 1, 1, 1);
    visibles.push_back(&surroundingCube);
    SetGoalPosition(Vector4(6, 6, 6, 6));
}
