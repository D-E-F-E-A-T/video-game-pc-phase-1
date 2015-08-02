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
	// Detect the gridspaces that are close to the player.
}