#include "Physics4D.h"

Colorization::Colorization(ColorSheme colorSheme, sf::Color color1, sf::Color color2) :
	colorScheme(colorSheme), color1(color1.toInteger()), color2(color2.toInteger())
{ }
Colorization::Colorization(ColorSheme colorSheme, unsigned color1, unsigned color2) :
	colorScheme(colorSheme), color1(color1), color2(color2)
{ }
