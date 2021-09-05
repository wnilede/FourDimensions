#include "GUI.h"

#include <iostream>

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text, sf::Font font, void (*ClickAction)(sf::RenderWindow& window), sf::RenderWindow& window) :
	position(position), size(size), text(text), font(font), ClickAction(ClickAction), color(sf::Color(50, 50, 5)), textColor(sf::Color(150, 150, 200)), window(window)
{ }
bool Button::Inside(sf::Vector2f position) const
{
	return abs(position.x - this->position.x) <= size.x / 2 && abs(position.y - this->position.y) <= size.y / 2;
}
void Button::Draw() const
{
	sf::Vector2f absoluteSize(size.x * window.getSize().x, size.y * window.getSize().y);
	sf::Vector2f absolutePosition(position.x * window.getSize().x, position.y * window.getSize().y);
	sf::RectangleShape rectangle{ absoluteSize };
	rectangle.setOrigin(sf::Vector2f(absoluteSize.x / 2, absoluteSize.y / 2));
	rectangle.setPosition(absolutePosition);
	rectangle.setFillColor(color);
	window.draw(rectangle);
	sf::Text textElement;
	textElement.setFont(font);
	textElement.setString(text);
	unsigned characterSize = (unsigned)std::min(textElement.getCharacterSize() * (absoluteSize.x - 10) / textElement.getLocalBounds().width, absoluteSize.y - 20);
	textElement.setCharacterSize(characterSize);
	textElement.setOrigin(textElement.getLocalBounds().width / 2, characterSize * 0.65f);
	textElement.setPosition(absolutePosition);
	textElement.setFillColor(textColor);
	window.draw(textElement);
}
