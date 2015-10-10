#pragma once
#include "pch.h"
#include "CollisionDetectionInfo.h"
#include <vector>
#include "Player.h"
#include "BaseSpriteData.h"
#include <list>
#include "GridSpace.h"

using namespace std;

// Possible strategies:
//	1. Character bounding box enters obstacle bounding box.
//		This would have 4-way directionality (North, East, South, West)
//	2. Character circular zone enters circular zone of the obstacle.
//		This would have 360-degrees of directionality (More than is needed for this game)
//	3. Character sprite overlaps with obstacle sprite.
//		More difficult to come up with directionality.
//
//	Directionality is needed to stop character from moving forward 
//	in the direction of the collision.
//
//	Not all sprites will block the character.
class CollisionDetectionStrategy
{
public:
	CollisionDetectionStrategy(){}

	virtual bool Detect(CollisionDetectionInfo * info) = 0;

	virtual void Detect(
		list<BaseSpriteData *> * retVal,
		float2 playerSize,
		float2 spriteSize,
		Player * pPlayer,
		vector<BaseSpriteData *> * sprites,
		float fWindowWidth,
		float fWindowHeight) = 0;

protected:

private:
};