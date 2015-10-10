#include "pch.h"
#include "BroadCollisionStrategy.h"
#include <vector>
#include "Player.h"
#include "MathUtils.h"

// @see http://www.gamedev.net/page/resources/_/technical/directx-and-xna/pixel-perfect-collision-detection-in-directx-r2939
BroadCollisionStrategy::BroadCollisionStrategy()
{

}

bool BroadCollisionStrategy::Detect(CollisionDetectionInfo * info)
{
	return false;
}

void BroadCollisionStrategy::Detect(
	list<BaseSpriteData *> * retVal,
	float2 playerSize,
	float2 spriteSize,
	Player * pPlayer,
	vector<BaseSpriteData *> * sprites,
	float fWindowWidth,
	float fWindowHeight)
{
	// Determine the 9 grid spaces around the player's location.
	Calculate(
		pPlayer, 
		sprites, 
		retVal,
		fWindowWidth,
		fWindowHeight);
}

int BroadCollisionStrategy::Calculate(
	Player * player, 
	vector<BaseSpriteData *> * sprites, 
	list<BaseSpriteData *> * retVal,
	float fWindowWidth,
	float fWindowHeight)
{
	int nCurrentHorizontalSpace = player->GetGridLocation()[HORIZONTAL_AXIS];
	int nCurrentVerticalSpace = player->GetGridLocation()[VERTICAL_AXIS];

	// Don't use grid spaces as locations since sprites might not be
	//	aligned within a grid space (i.e. moving sprites)

	std::vector<BaseSpriteData *>::const_iterator iterator;

	for (iterator = sprites->begin(); iterator != sprites->end(); iterator++)
	{
		BaseSpriteData * sprite = (*iterator);

		if (IsClose(player, sprite, fWindowWidth, fWindowHeight))
		{
			retVal->push_back(sprite);
		}
	}

	return 1;
}

boolean BroadCollisionStrategy::IsClose(
	Player * player, 
	BaseSpriteData * data, 
	float fWindowWidth,
	float fWindowHeight)
{
	float distance = CalculateDistance(
		*player, 
		data, 
		fWindowWidth, 
		fWindowHeight);

	return (distance < 100.f);		
}

float BroadCollisionStrategy::CalculateDistance(
	Player player, 
	BaseSpriteData * sprite,
	float fWindowWidth,
	float fWindowHeight)
{
	float playerLocation[2];
	float spriteLocation[2];

	// These are within the range of screen pixel size.
	playerLocation[0] = player.GetHorizontalRatio() * fWindowWidth;
	playerLocation[1] = player.GetVerticalRatio() * fWindowHeight;

	// These are within the range of screen pixel size.
	spriteLocation[0] = sprite->pos.x;
	spriteLocation[1] = sprite->pos.y;

	return MathUtils::CalculateDistance(
		playerLocation[0],
		playerLocation[1],
		spriteLocation[0],
		spriteLocation[1]);
}

