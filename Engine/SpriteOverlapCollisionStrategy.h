#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"
#include "Player.h"
#include "BaseSpriteData.h"
#include "GridSpace.h"

class SpriteOverlapCollisionStrategy : public CollisionDetectionStrategy
{
public:
	SpriteOverlapCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);

	void Detect(
		list<BaseSpriteData> * retVal,
		float2 playerSize,
		float2 spriteSize,
		Player * pPlayer,
		vector<BaseSpriteData> * sprites,
		float fWindowWidth,
		float fWindowHeight);

protected:
	int BroadStrategy(Player * player, list<BaseSpriteData *> * retVal);
	int NarrowStrategy(Player * player, list<BaseSpriteData *> * retVal);

private:
};