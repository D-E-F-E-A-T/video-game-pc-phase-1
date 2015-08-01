#include "pch.h"
#include "BaseGridSpace.h"

using namespace Microsoft::WRL;

class Grid
{
public:
	Grid();
	~Grid();

	void SetWindowWidth(float fWidth);
	void SetWindowHeight(float fHeight);

	void Draw(
		ComPtr<ID2D1DeviceContext1> context, 
		ComPtr<ID2D1SolidColorBrush> brush);

	float GetColumnWidth();
	float GetRowHeight();

protected:

private:
	float m_fWindowWidth;
	float m_fWindowHeight;
	float m_fGridWidth;
	float m_fGridHeight;
};