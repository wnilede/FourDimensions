#include "Physics4D.h"

Colorization::Colorization(sf::Color color) :
	colorScheme(ColorSheme::simple), color1(color.toInteger()), color2(color.toInteger())
{ }
Colorization::Colorization(unsigned color) :
	colorScheme(ColorSheme::simple), color1(color), color2(color)
{ }
Colorization::Colorization(ColorSheme colorSheme, sf::Color color1, sf::Color color2) :
	colorScheme(colorSheme), color1(color1.toInteger()), color2(color2.toInteger())
{ }
Colorization::Colorization(ColorSheme colorSheme, unsigned color1, unsigned color2) :
	colorScheme(colorSheme), color1(color1), color2(color2)
{ }
