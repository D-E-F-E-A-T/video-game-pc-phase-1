#pragma once
#include "pch.h"
#include <BasicMath.h>
#include "BaseSpriteData.h"

struct OrchiData : public BaseSpriteData
{
public:
	OrchiData() : BaseSpriteData(0, 0, 0.0f, 0.0f)
	{
		
	}

	OrchiData(int column, int row, float x, float y) : BaseSpriteData(column, row, x, y)
	{

	}

protected:

private:
};