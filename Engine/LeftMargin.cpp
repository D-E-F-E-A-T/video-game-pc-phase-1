#include "pch.h"
#include "LeftMargin.h"
#include "Constants.h"

using namespace Microsoft::WRL;

LeftMargin::LeftMargin()
{
}

void LeftMargin::Draw(
	ComPtr<ID2D1DeviceContext1> context, 
	ComPtr<ID2D1SolidColorBrush> brush,
	D2D1_RECT_F rect)
{
	context->FillRectangle(
		rect,
		brush.Get());
}