#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"
#include "Player.h"
#include <list>
#include "GridSpace.h"

class BoundingBoxCornerCollisionStrategy : public CollisionDetectionStrategy
{
public:
	BoundingBoxCornerCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);
	void Detect(
		list<GridSpace *> * retVal,
		float2 playerSize,
		float2 spriteSize,
		Player * pPlayer,
		vector<BaseSpriteData> * sprites);

protected:

private:
};