#include "GUI.h"

#include <iostream>
#include "World.h"

using namespace std::string_literals;

Menu::Menu(std::vector<Button> buttons, sf::RenderWindow& window) : buttons(buttons), backgroundColor(sf::Color(10, 10, 200)), window(window)
{ }
void Menu::Run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    for (const Button& button : buttons)
                    {
                        if (button.Inside(sf::Vector2f((float)event.mouseButton.x / window.getSize().x, (float)event.mouseButton.y / window.getSize().y))) {
                            button.ClickAction(window);
                        }
                    }
                }
                break;
            }
        }
        window.clear();
        for (const Button& button : buttons) {
            button.Draw();
        }
        window.display();
    }
}
Menu Menu::GetStartMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cout << "No font file found." << std::endl;
    }
    Button TestingWorldButton(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.2f, 0.2f), "Testing World"s, font, [](sf::RenderWindow& window){
        TestingWorld world{ };
        world.Run(window);
    }, window);
    std::vector<Button> buttons;
    buttons.push_back(TestingWorldButton);
    return Menu{ buttons, window };
}
