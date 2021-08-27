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
    TestingWorld world{ };
    world.Run();
    return 0;
}
