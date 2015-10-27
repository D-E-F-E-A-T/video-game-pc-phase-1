#include "pch.h"
#include "Grid.h"
#include "Constants.h"

Grid::Grid()
{

}

Grid::~Grid()
{

}

void Grid::SetWindowWidth(float fWindowWidth)
{
	this->m_fWindowWidth = fWindowWidth;

	m_fGridWidth = m_fWindowWidth -
		(m_fWindowWidth * LEFT_MARGIN_RATIO) -
		(m_fWindowWidth * RIGHT_MARGIN_RATIO);
}

void Grid::SetWindowHeight(float fWindowHeight)
{
	this->m_fWindowHeight = fWindowHeight;

	m_fGridHeight = fWindowHeight;
}

void Grid::Draw(
	ComPtr<ID2D1DeviceContext1> context,
	ComPtr<ID2D1SolidColorBrush> brush)
{
	if (m_bIsVisible)
	{
		// Draw the horizontal lines.
		for (int row = 0; row <= NUM_GRID_ROWS; row++)
		{
			D2D1_POINT_2F src
			{
				(m_fWindowWidth * LEFT_MARGIN_RATIO) + MARGIN,
				MARGIN + (GetRowHeight() * (float)row)
			};

			D2D1_POINT_2F dst
			{
				m_fWindowWidth - (m_fWindowWidth * RIGHT_MARGIN_RATIO) - MARGIN,
				MARGIN + (GetRowHeight() * (float)row)
			};

			context->DrawLine(src, dst, brush.Get());
		}

		// Draw the vertical lines.
		for (int column = 0; column <= NUM_GRID_COLUMNS; column++)
		{
			D2D1_POINT_2F src
			{
				(m_fWindowWidth * LEFT_MARGIN_RATIO) + MARGIN + (GetColumnWidth() * (float)column),
				MARGIN
			};

			D2D1_POINT_2F dst
			{
				(m_fWindowWidth * LEFT_MARGIN_RATIO) + MARGIN + (GetColumnWidth() * (float)column),
				m_fWindowHeight - MARGIN,
			};

			context->DrawLine(src, dst, brush.Get());
		}
	}
}

float Grid::GetColumnWidth()
{
	return (m_fGridWidth - 2.0f * MARGIN) / NUM_GRID_COLUMNS;
}

float Grid::GetRowHeight()
{
	return (m_fWindowHeight - 2.0f * MARGIN) / NUM_GRID_ROWS;
}

void Grid::SetNumColumns(int nNumColumns)
{
	this->m_nNumColumns = nNumColumns;
}

void Grid::SetNumRows(int nNumRows)
{
	this->m_nNumRows = nNumRows;
}

void Grid::SetVisibility(boolean bVisibility)
{
	this->m_bIsVisible = bVisibility;
}
