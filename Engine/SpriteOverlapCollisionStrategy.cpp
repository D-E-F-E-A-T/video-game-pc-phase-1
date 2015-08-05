#include "pch.h"
#include "SpriteOverlapCollisionStrategy.h"
#include <vector>
#include "Player.h"

// @see http://www.gamedev.net/page/resources/_/technical/directx-and-xna/pixel-perfect-collision-detection-in-directx-r2939
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
	vector<BaseSpriteData> * sprites,
	float fWindowWidth,
	float fWindowHeight)
{
	// Determine the 9 grid spaces around the player's location.
	BroadStrategy(pPlayer, retVal);
}

int SpriteOverlapCollisionStrategy::BroadStrategy(Player * player, list<GridSpace *> * retVal)
{
	int nCurrentHorizontalSpace = player->GetGridLocation()[HORIZONTAL_AXIS];
	int nCurrentVerticalSpace = player->GetGridLocation()[VERTICAL_AXIS];

	retVal->push_back(new GridSpace(nCurrentHorizontalSpace - 1, nCurrentVerticalSpace - 1));
	retVal->push_back(new GridSpace(nCurrentHorizontalSpace, nCurrentVerticalSpace - 1));
	retVal->push_back(new GridSpace(nCurrentHorizontalSpace + 1, nCurrentVerticalSpace - 1));

	retVal->push_back(new GridSpace(nCurrentHorizontalSpace - 1, nCurrentVerticalSpace));
	retVal->push_back(new GridSpace(nCurrentHorizontalSpace, nCurrentVerticalSpace));
	retVal->push_back(new GridSpace(nCurrentHorizontalSpace + 1, nCurrentVerticalSpace));

	retVal->push_back(new GridSpace(nCurrentHorizontalSpace - 1, nCurrentVerticalSpace + 1));
	retVal->push_back(new GridSpace(nCurrentHorizontalSpace, nCurrentVerticalSpace + 1));
	retVal->push_back(new GridSpace(nCurrentHorizontalSpace + 1, nCurrentVerticalSpace + 1));

	return 1;
}

int SpriteOverlapCollisionStrategy::NarrowStrategy(Player * player, list<GridSpace *> * retVal)
{
	return NULL;
}