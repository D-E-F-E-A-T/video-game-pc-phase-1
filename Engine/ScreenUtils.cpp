#include "pch.h"
#include "ScreenUtils.h"
#include "Constants.h"

void ScreenUtils::CalculateSquareCenter(
	float screenWidth,
	float screenHeight,
	int column,
	int row,
	float * x,
	float * y)
{
	float gridWidth = screenWidth -
		(screenWidth * LEFT_MARGIN_RATIO) -
		(screenWidth * RIGHT_MARGIN_RATIO);

	float rowHeight = (screenHeight - 2.0f * MARGIN) / NUM_GRID_ROWS;
	float columnWidth = (gridWidth - 2.0f * MARGIN) / NUM_GRID_COLUMNS;

	*x = (screenWidth * LEFT_MARGIN_RATIO) + MARGIN + (columnWidth * column) + (columnWidth / 2.0f);
	*y = MARGIN + (rowHeight * row) + (rowHeight / 2.0f);
}