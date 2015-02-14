#include "pch.h"
#include "Tree.h"

Tree::Tree()
{

}

Tree::~Tree()
{

}

// Initialize the values within the base class contructor.
Tree::Tree(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush) : 
	BaseGridSpace(row, column, x, y, brush)
{
}

void Tree::Draw(ComPtr<ID2D1DeviceContext1> context)
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


