#include "pch.h"
#include "BoundingBoxMidpointCollisionStrategy.h"
#include "Player.h"
#include "BaseSpriteData.h"

BoundingBoxMidpointCollisionStrategy::BoundingBoxMidpointCollisionStrategy()
{

}

bool BoundingBoxMidpointCollisionStrategy::Detect(CollisionDetectionInfo * info)
{
	return false;
}

void BoundingBoxMidpointCollisionStrategy::Detect(
	list<GridSpace *> * retVal,
	float2 playerSize,
	float2 spriteSize,
	Player * pPlayer,
	vector<BaseSpriteData> * sprites,
	float fWindowWidth, 
	float fWindowHeight)
{
	float left = fWindowWidth * pPlayer->GetHorizontalRatio() - playerSize.x / 2.0f;
	float right = fWindowWidth * pPlayer->GetHorizontalRatio() + playerSize.x / 2.0f;
	float top = fWindowHeight * pPlayer->GetVerticalRatio() - playerSize.y / 2.0f;
	float bottom = fWindowHeight * pPlayer->GetVerticalRatio() + playerSize.y / 2.0f;

	// Now I know the size (remember to scale accordingly, if needed using m_orchiData).

	for (auto tree = sprites->begin(); tree != sprites->end(); tree++)
	{
		float treeLeft = tree->pos.x - spriteSize.x / 2.0f;
		float treeRight = tree->pos.x + spriteSize.x / 2.0f;
		float treeTop = tree->pos.y - spriteSize.y / 2.0f;
		float treeBottom = tree->pos.y + spriteSize.y / 2.0f;

		// Does the top, left vertex overlap the tree's bounding box?
		if (left >= treeLeft &&
			left <= treeRight &&
			top >= treeTop &&
			top <= treeBottom)
		{
			retVal->push_back(new GridSpace(tree->column, tree->row));
		}

		// Does the top, right vertex overlap the tree's bounding box?
		if (right >= treeLeft &&
			right <= treeRight &&
			top >= treeTop &&
			top <= treeBottom)
		{
			retVal->push_back(new GridSpace(tree->column, tree->row));
		}

		// Does the bottom, right vertex overlap the tree's bounding box?
		if (right >= treeLeft &&
			right <= treeRight &&
			bottom >= treeTop &&
			bottom <= treeBottom)
		{
			retVal->push_back(new GridSpace(tree->column, tree->row));
		}

		// Does the bottom, left vertex overlap the tree's bounding box?
		if (left >= treeLeft &&
			left <= treeRight &&
			bottom >= treeTop &&
			bottom <= treeBottom)
		{
			retVal->push_back(new GridSpace(tree->column, tree->row));
		}
	}
}
