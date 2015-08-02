#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"
#include "Player.h"
#include "BaseSpriteData.h"
#include <list>
#include "GridSpace.h"

class CircularZoneCollisionStrategy : public CollisionDetectionStrategy
{
public:
	CircularZoneCollisionStrategy();
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