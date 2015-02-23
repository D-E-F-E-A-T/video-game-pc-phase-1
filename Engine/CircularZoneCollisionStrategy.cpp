#include "pch.h"
#include "CircularZoneCollisionStrategy.h"

using namespace std;

CircularZoneCollisionStrategy::CircularZoneCollisionStrategy()
{
}

bool CircularZoneCollisionStrategy::Detect(CollisionDetectionInfo * info)
{
	return false;
}

int CircularZoneCollisionStrategy::Detect(
	int * column,
	int * row,
	float2 playerSize,
	float2 spriteSize,
	Player * pPlayer,
	vector<BaseSpriteData> * sprites)
{
	return 0;
}