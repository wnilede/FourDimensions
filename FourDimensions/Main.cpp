#include <iostream>
#include <limits>
#include <cmath>
#include <execution>
#include <algorithm>

#include "Linjear4D.h"
#include "Physics4D.h"
#include "RayCasting.cuh"
#include "World.h"
#include "GUI.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "4D-game", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    Menu startMenu{ Menu::GetStartMenu(window) };
    startMenu.Run();
    return 0;
}
