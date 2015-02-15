#include "pch.h"
#include "Water.h"

Water::Water()
{

}

Water::~Water()
{

}

// Initialize the values within the base class contructor.
Water::Water(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush) :
	BaseGridSpace(row, column, x, y, brush)
{
}

void Water::Draw(ComPtr<ID2D1DeviceContext1> context)
{
	D2D1_RECT_F rect
	{
		this->x - 10.0f,
		this->y - 10.0f,
		this->x + 10.0f,
		this->y + 10.0f
	};

	context->DrawRectangle(
		rect,
		brush.Get());
}


