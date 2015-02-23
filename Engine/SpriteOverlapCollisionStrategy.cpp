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

int SpriteOverlapCollisionStrategy::Detect(
	int * column,
	int * row,
	float2 playerSize,
	float2 spriteSize,
	Player * pPlayer,
	vector<BaseSpriteData> * sprites)
{
	return 0;
}