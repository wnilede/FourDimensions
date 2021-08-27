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
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "4D-game", sf::Style::Titlebar | sf::Style::Close);
    world.Run(window);
    return 0;
}
