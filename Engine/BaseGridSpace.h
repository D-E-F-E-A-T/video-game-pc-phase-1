#pragma once

#include "pch.h"

using namespace Microsoft::WRL;

class BaseGridSpace
{
public:
	BaseGridSpace()
	{

	}

	BaseGridSpace(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush)
	{
		this->row = row;
		this->column = column;
		this->x = x;
		this->y = y;
		this->brush = brush;
	}

	virtual void Draw(ComPtr<ID2D1DeviceContext1> context) = 0;

private:

protected:
	int row;
	int column;
	float x;
	float y;
	ComPtr<ID2D1SolidColorBrush> brush;

	// Could have a super class for Tree, Rock, etc to facilitate Flyweight.

};