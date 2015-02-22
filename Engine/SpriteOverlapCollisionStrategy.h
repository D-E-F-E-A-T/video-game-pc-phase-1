#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"

class SpriteOverlapCollisionStrategy : public CollisionDetectionStrategy
{
public:
	SpriteOverlapCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);

protected:

private:
};