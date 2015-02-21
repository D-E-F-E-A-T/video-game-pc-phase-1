#pragma once
#include "pch.h"

using namespace Microsoft::WRL;

class RightMargin
{
public:
	RightMargin();
	void Draw(
		ComPtr<ID2D1DeviceContext1> context,
		ComPtr<ID2D1SolidColorBrush> brush,
		D2D1_RECT_F rect);

protected:

private:
};