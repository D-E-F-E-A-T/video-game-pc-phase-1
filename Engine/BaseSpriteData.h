#pragma once
#include "pch.h"

class BaseSpriteData
{
public:
	int row;
	int column;
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
	bool bBlockable;
	bool bCollidable;

	BaseSpriteData(int column, int row, float x, float y)
	{
		this->column = column;
		this->row = row;
		this->pos.x = x;
		this->pos.y = y;

		this->rot = 0.0f;
		this->scale = 1.0f;
		this->rotVel = 0.0f;

		this->bBlockable = true;
	}

	void SetBlockable(bool blockable)
	{
		this->bBlockable = blockable;
	}

protected:

private:

};