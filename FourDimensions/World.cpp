#include <iostream>
#include <chrono>
#include <windows.h>
#include <processthreadsapi.h>

#define SCREEN_HEIGHT 1000
#define SCREEN_WIDTH 1200

#include "World.h"

//Should not be needed to define FPN since Linjear4D.h is included indirectly, but IntelliSense requires it.
#ifndef FPN
#define FPN float
#endif

World::World() :
    window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "4D-game", sf::Style::Titlebar | sf::Style::Close),
    mouseScrollSensitivity((FPN)0.01), mouseMoveSensitivity((FPN)0.005),
    gravitationalAcceleration((FPN)constants::standardGravity),
    soundController{},
    rayCaster(window, player, visibles, visiblesPlayerUpsMutex, SCREEN_HEIGHT, SCREEN_WIDTH, lockTest)
{
    if (!font.loadFromFile("verdana.ttf")) {
        std::cout << "No font file found." << std::endl;
    }
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);
}
void World::UpdatePhysics()
{
    std::scoped_lock lock{ visiblesPlayerUpsMutex };

    if (lockTest.load())
        std::cout << "Lock is strange 2\n\r";
    lockTest.store(true);

    sf::Time elapsedTime = physicsClock.restart();
    ups = elapsedTime;//(elapsedTime + ups) / (float)2;
    FPN mouseScroll = 0;
    const sf::Vector2i centerOfScreen = sf::Vector2i(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
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

    player.HandleViewControlls(
        mouseMovement.x * mouseMoveSensitivity,
        -mouseMovement.y * mouseMoveSensitivity,
        -mouseScroll);

    lockTest.store(false);
}
bool World::IsOpen()
{
    return window.isOpen() && !closing;
}
void World::StartPhysicsLoop()
{
    std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
    physicsClock.restart();
    while (IsOpen())
    {
        //Sleep(max((targetUpdateInterval - physicsClock.getElapsedTime()).asMilliseconds(), 0));
        //std::this_thread::sleep_for(std::chrono::microseconds(max(((targetUpdateInterval - physicsClock.getElapsedTime()) * (float)0.7).asMicroseconds(), 0))); //Sleeps for some random amount of time perhaps close to the wanted one.
        //while (physicsClock.getElapsedTime() < targetUpdateInterval)
        //    std::this_thread::sleep_for(std::chrono::microseconds(2000));
        //std::this_thread::sleep_until(lastUpdate + std::chrono::microseconds(targetUpdateInterval.asMicroseconds()) * 0.7);
        sf::sleep(targetUpdateInterval - physicsClock.getElapsedTime());
        lastUpdate = std::chrono::steady_clock::now();
        UpdatePhysics();
    }
}
void World::StartDrawLoop()
{
    while (window.isOpen())
    {
        sf::Time elapsedTime = graphicsClock.restart();
        fps = (elapsedTime + fps) / (float)2;
        text.setFont(font);
        window.clear();
        {
            sf::Clock clock;
            std::scoped_lock<std::mutex> lock{ visiblesPlayerUpsMutex };
            if (lockTest.load())
                std::cout << "Lock is strange 1\n\r";
            lockTest = true;
            //rayCaster.RayCastScreen();
            rayCaster.pVisiblesImage = new VisiblesImage(visibles, player);
            text.setString("FPS: " + std::to_string(1 / fps.asSeconds()) + "\n\rUPS: " + std::to_string(1 / ups.asSeconds()) + "\n\rLocked: " + std::to_string(clock.getElapsedTime().asSeconds()));
            lockTest = false;
        }
        rayCaster.RayCastScreen();
        delete rayCaster.pVisiblesImage;
        window.draw(text);
        window.display();
        if (closing)
            window.close();
    }
}
void World::Run()
{
    window.setActive(false);
    std::thread drawThread = std::thread([this] {
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
        SetThreadDescription(GetCurrentThread(), L"Graphics Thread");
        this->StartDrawLoop();
    });
    StartPhysicsLoop();
    drawThread.join();
}
