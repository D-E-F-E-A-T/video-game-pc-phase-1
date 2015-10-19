#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"
#include "Player.h"
#include "BaseSpriteData.h"
#include "GridSpace.h"

class BroadCollisionStrategy // : public CollisionDetectionStrategy
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
		float fWindowHeight,
		float * playerLocation);

protected:
	int Calculate(
		Player * player, 
		vector<BaseSpriteData *> * sprites, 
		list<BaseSpriteData *> * retVal,
		float fWindowWidth,
		float fWindowHeight,
		float * playerLocation);

	boolean IsClose(
		Player * player, 
		BaseSpriteData * data,
		float fWindowWidth,
		float fWindowHeight,
		float * playerLocation);

	float CalculateDistance(
		Player player, 
		BaseSpriteData * sprite, 
		float fWindowWidth, 
		float fWindowHeight,
		float * playerLocation);

private:
};