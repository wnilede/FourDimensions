#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct Button
{
	sf::Vector2f position;
	sf::Vector2f size;
	std::string text;
	sf::Font font;
	sf::Color color;
	sf::Color textColor;
	sf::RenderWindow& window;
	void (*ClickAction)(sf::RenderWindow& window);
	Button(sf::Vector2f position, sf::Vector2f size, std::string text, sf::Font font, void (*ClickAction)(sf::RenderWindow& window), sf::RenderWindow& window);
	bool Inside(sf::Vector2f position) const;
	void Draw() const;
};
struct Menu
{
	std::vector<Button> buttons;
	sf::Color backgroundColor;
	sf::RenderWindow& window;
	Menu(std::vector<Button> buttons, sf::RenderWindow& window);
	void Run();
	static Menu GetStartMenu(sf::RenderWindow& window);
};
