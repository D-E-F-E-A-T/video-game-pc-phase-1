#pragma once
#include "pch.h"
#include "CollisionDetectionStrategy.h"

class BoundingBoxCollisionStrategy : public CollisionDetectionStrategy
{
public:
	BoundingBoxCollisionStrategy();
	bool Detect(CollisionDetectionInfo * info);

protected:

private:
};