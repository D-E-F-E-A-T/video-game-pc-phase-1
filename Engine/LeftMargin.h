#pragma once
#include "pch.h"

using namespace Microsoft::WRL;

class LeftMargin
{
public:
	LeftMargin();
	void Draw(
		ComPtr<ID2D1DeviceContext1> context,
		ComPtr<ID2D1SolidColorBrush> brush,
		D2D1_RECT_F rect);

protected:

private:


};