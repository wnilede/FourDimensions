#include <iostream>
#include <chrono>
#include <windows.h>
#include <processthreadsapi.h>

#include "World.h"

//Should not be needed to define FPN since Linjear4D.h is included indirectly, but IntelliSense requires it.
#ifndef FPN
#define FPN float
#endif

World::World() :
    mouseScrollSensitivity((FPN)0.01), mouseMoveSensitivity((FPN)0.005), gravitationalAcceleration((FPN)constants::standardGravity), soundController{},
    goal(Mesh::GetCube(Vector4(), Rotation(), 1, Colorization(ColorSheme::dragedCubes, 0xFFFFFFFF, 0x000000FF)), lifetimeClock, Vector4(1, 1, 0, 0), Vector4(0, 0, 1, 1), (FPN)constants::pi / 4, 1)
{
    if (!font.loadFromFile("verdana.ttf")) {
        std::cout << "No font file found." << std::endl;
    }
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    visibles.push_back(&goal);
    updatables.push_back(&goal);
}
void World::SetGoalPosition(Vector4 position)
{
    goal.setPosition(position);
}
void World::UpdatePhysics(sf::RenderWindow& window, RayCaster& rayCaster)
{
    std::scoped_lock lock{ visiblesPlayerMutex };

    sf::Time elapsedTime = physicsClock.restart();
    {
        std::scoped_lock<std::mutex> lock{ debugInfoMutex };
        timePerUpdate = elapsedTime;
    }
    FPN mouseScroll = 0;
    const sf::Vector2i centerOfScreen = sf::Vector2i(window.getSize().y / 2, window.getSize().x / 2);
    const sf::Vector2i mouseMovement = sf::Mouse::getPosition(window) - centerOfScreen;
    if (window.hasFocus()) {
        sf::Mouse::setPosition(centerOfScreen, window);
    }

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            closing = true;
            break;

        case sf::Event::MouseWheelScrolled:
            mouseScroll += (FPN)event.mouseWheelScroll.delta * mouseScrollSensitivity;
            break;

        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F2)
            {
                std::scoped_lock<std::mutex> lock{ debugInfoMutex };
                showDebugInfo = !showDebugInfo;
            }
            break;

        default:
            break;
        }
    }

    for (Updatable* updatable : updatables) {
        updatable->Update();
    }



    //NEW PLAN FOR PHYSICS UPDATE FOR PLAYER:
    //Move horizontally according to keyboard
    //Change speed from gravitational acceleration
    //Move vertically in speed
    //Raycast in all directions
    //If inside something:
        //Move out
        //Remove speed or set speed from jump
    //Handle view controlls



    Vector4 directionGoing{};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        directionGoing += player.forward;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        directionGoing -= player.forward;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        directionGoing += player.GetScreenXVector();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        directionGoing -= player.GetScreenXVector();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        directionGoing += player.GetScreenYVector();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        directionGoing -= player.GetScreenYVector();
    if (directionGoing != Vector4{}) {
        player.position += directionGoing.GetNormalized() * player.movementSpeed * elapsedTime.asSeconds();
    }
    player.verticalSpeed -= gravitationalAcceleration * elapsedTime.asSeconds();
    player.position.W += player.verticalSpeed * elapsedTime.asSeconds();

    Vector4 hitVector{ 1, 2, 3, 4 };
    FPN minDistance = std::numeric_limits<FPN>().infinity();
    for (const Vector4& vector : player.shape) {
        FPN distance = rayCaster.RayCastCPU(player.position, vector) - vector.GetLength();
        if (distance < minDistance) {
            minDistance = distance;
            hitVector = vector;
        }
    }
    if (minDistance < 0)
    {
        player.position += hitVector.GetNormalized() * minDistance; //Maybe this should be done for more than one vector that hit
        if (Vector4::AngleBetween(hitVector, Vector4{ 0, 0, 0, -1 }) < (FPN)constants::pi / 3)
        {
            if (player.verticalSpeed < -2)
                soundController.PlaySoun(Sound::Landing, -player.verticalSpeed * 12);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                player.verticalSpeed = player.jumpSpeed;
                soundController.PlaySoun(Sound::Jump);
            }
            else
                player.verticalSpeed = 0;
        }
    }

    if ((goal.getPosition() - player.position).GetLength() < 3) {
        closing = true;
    }

    player.HandleViewControlls(
        mouseMovement.x * mouseMoveSensitivity,
        -mouseMovement.y * mouseMoveSensitivity,
        -mouseScroll);
}
void World::StartPhysicsLoop(sf::RenderWindow& window, RayCaster& rayCaster)
{
    std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
    physicsClock.restart();
    while (window.isOpen() && !closing)
    {
        //Sleep(max((targetUpdateInterval - physicsClock.getElapsedTime()).asMilliseconds(), 0));
        //std::this_thread::sleep_for(std::chrono::microseconds(max(((targetUpdateInterval - physicsClock.getElapsedTime()) * (float)0.7).asMicroseconds(), 0))); //Sleeps for some random amount of time perhaps close to the wanted one.
        //while (physicsClock.getElapsedTime() < targetUpdateInterval)
        //    std::this_thread::sleep_for(std::chrono::microseconds(2000));
        //std::this_thread::sleep_until(lastUpdate + std::chrono::microseconds(targetUpdateInterval.asMicroseconds()) * 0.7);
        sf::sleep(targetUpdateInterval - physicsClock.getElapsedTime());
        lastUpdate = std::chrono::steady_clock::now();
        UpdatePhysics(window, rayCaster);
    }
}
void World::StartDrawLoop(sf::RenderWindow& window, RayCaster& rayCaster)
{
    while (window.isOpen())
    {
        timePerFrame = graphicsClock.restart();
        window.clear();
        {
            std::scoped_lock<std::mutex> lock{ visiblesPlayerMutex };
            rayCaster.pVisiblesImage = new VisiblesImage(visibles, player);
        }
        rayCaster.RayCastScreen();
        delete rayCaster.pVisiblesImage;
        {
            std::scoped_lock<std::mutex> lock{ debugInfoMutex };
            if (showDebugInfo) {
                text.setFont(font);
                text.setString("FPS: " + std::to_string(1 / timePerFrame.asSeconds()) + "\n\rUPS: " + std::to_string(1 / timePerUpdate.asSeconds()));
                window.draw(text);
            }
        }
        window.display();
        if (closing)
            window.close();
    }
}
void World::Run(sf::RenderWindow& window)
{
    RayCaster rayCaster{ window, player, visibles, visiblesPlayerMutex, window.getSize().y, window.getSize().x };

    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    window.setActive(false);
    physicsClock.restart();
    std::thread drawThread = std::thread([this, &window, &rayCaster] {
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
        SetThreadDescription(GetCurrentThread(), L"Graphics Thread");
        this->StartDrawLoop(window, rayCaster);
    });
    StartPhysicsLoop(window, rayCaster);
    drawThread.join();
}
