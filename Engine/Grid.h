#pragma once
#include "pch.h"
#include "BaseGridSpace.h"
#include "Constants.h"

using namespace Microsoft::WRL;

class Grid
{
public:
	Grid();
	~Grid();

	void SetWindowWidth(float fWidth);
	void SetWindowHeight(float fHeight);
	
	void SetNumColumns(int nColumns);
	void SetNumRows(int nRows);
	void SetVisibility(boolean bVisibility);

	void Draw(
		ComPtr<ID2D1DeviceContext1> context, 
		ComPtr<ID2D1SolidColorBrush> brush);

	float GetColumnWidth();
	float GetRowHeight();

	int GetNumRows() 
	{
		return m_nNumRows;
	}

	int GetNumColumns()
	{
		return m_nNumColumns;
	}

protected:

private:
	float m_fWindowWidth;
	float m_fWindowHeight;
	float m_fGridWidth;
	float m_fGridHeight;

	int m_nNumColumns;
	int m_nNumRows;

	boolean m_bIsVisible;
};