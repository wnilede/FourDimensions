#include "Physics4D.h"

bool RayCastResult::Success()
{
	return distance >= 0 && isfinite(distance);
}
RayCastResult::RayCastResult(FPN distance, sf::Color color) : distance(distance), color(color) { }
