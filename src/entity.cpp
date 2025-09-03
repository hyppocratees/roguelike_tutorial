#include "entity.h"
#include <cmath>

float Entity::Distance(int x, int y) const
{
	return sqrt((x - x_) * (x - x_) + (y - y_) * (y - y_));
}
