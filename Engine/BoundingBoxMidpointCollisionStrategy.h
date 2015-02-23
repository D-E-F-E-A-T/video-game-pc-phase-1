#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"

class BoundingBoxMidpointCollisionStrategy : public CollisionDetectionStrategy
{
public:
	BoundingBoxMidpointCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);
	int Detect(
		int * column,
		int * row,
		float2 playerSize,
		float2 spriteSize,
		Player * pPlayer,
		vector<BaseSpriteData> * sprites);

protected:

private:
};