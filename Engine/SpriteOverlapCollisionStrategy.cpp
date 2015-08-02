#include "pch.h"
#include "SpriteOverlapCollisionStrategy.h"
#include <vector>
#include "Player.h"

SpriteOverlapCollisionStrategy::SpriteOverlapCollisionStrategy()
{

}

bool SpriteOverlapCollisionStrategy::Detect(CollisionDetectionInfo * info)
{
	return false;
}

void SpriteOverlapCollisionStrategy::Detect(
	list<GridSpace *> * retVal,
	float2 playerSize,
	float2 spriteSize,
	Player * pPlayer,
	vector<BaseSpriteData> * sprites)
{
}