#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"

class CircularZoneCollisionStrategy : public CollisionDetectionStrategy
{
public:
	CircularZoneCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);

protected:

private:
};