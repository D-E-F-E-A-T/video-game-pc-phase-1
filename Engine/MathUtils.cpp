#include "pch.h"
#include <math.h>
#include "MathUtils.h"

float MathUtils::CalculateDistance(
	float x1,
	float y1,
	float x2,
	float y2)
{
	float deltaX = fabs(x2 - x1);
	float deltaY = fabs(y2 - y1);

	return
		sqrt((deltaX * deltaX) + (deltaY * deltaY));
}