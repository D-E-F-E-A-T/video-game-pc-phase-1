#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"
#include "Player.h"
#include "BaseSpriteData.h"
#include "GridSpace.h"

class BroadCollisionStrategy : public CollisionDetectionStrategy
{
public:
	BroadCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);

	void Detect(
		list<BaseSpriteData *> * retVal,
		float2 playerSize,
		float2 spriteSize,
		Player * pPlayer,
		vector<BaseSpriteData *> * sprites,
		float fWindowWidth,
		float fWindowHeight);

protected:
	int Calculate(
		Player * player, 
		vector<BaseSpriteData *> * sprites, 
		list<BaseSpriteData *> * retVal,
		float fWindowWidth,
		float fWindowHeight);

	boolean IsClose(
		Player * player, 
		BaseSpriteData * data,
		float fWindowWidth,
		float fWindowHeight);

	float CalculateDistance(
		Player player, 
		BaseSpriteData * sprite, 
		float fWindowWidth, float fWindowHeight);

	float BroadCollisionStrategy::CalculateDistance(
		float x1,
		float y1,
		float x2,
		float y2);

private:
};