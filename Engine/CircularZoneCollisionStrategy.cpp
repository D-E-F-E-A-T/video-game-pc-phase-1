#include "pch.h"
#include "CircularZoneCollisionStrategy.h"
#include <list>

using namespace std;

CircularZoneCollisionStrategy::CircularZoneCollisionStrategy()
{
}

bool CircularZoneCollisionStrategy::Detect(CollisionDetectionInfo * info)
{
	return false;
}

void CircularZoneCollisionStrategy::Detect(
	list<BaseSpriteData> * retVal,
	float2 playerSize,
	float2 spriteSize,
	Player * pPlayer,
	vector<BaseSpriteData> * sprites,
	float fWindowWidth,
	float fWindowHeight)
{
}