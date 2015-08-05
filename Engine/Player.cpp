#include "pch.h"
#include "Player.h"
#include "Constants.h"

Player::Player(Grid * grid)
{
	m_grid = grid;
	m_fHorizontalRatio = 0.5f;
	m_fVerticalRatio = 0.5f;

	m_nUnitsPerGridSquare[HORIZONTAL_AXIS] =
		1.0f / grid->GetNumColumns() * TOTAL_GRID_DIVISIONS;

	m_nUnitsPerGridSquare[VERTICAL_AXIS] =
		1.0f / grid->GetNumRows() * TOTAL_GRID_DIVISIONS;

	UpdateGridLocation();
}

void Player::MoveNorth(float fVelocity)
{
	float prospectiveVerticalRatio =
		m_fVerticalRatio -= fVelocity;

	// Don't go above the top of the screen. 
	//	Later this will be the trigger to move to the next screen.
	if (prospectiveVerticalRatio >= 0.f)
		m_fVerticalRatio = prospectiveVerticalRatio;
	else
		m_fVerticalRatio = 0.0f;

	UpdateGridLocation();
}

void Player::MoveEast(float fVelocity)
{
	float prospectiveHorizontalRatio =
		m_fHorizontalRatio += fVelocity;

	if (prospectiveHorizontalRatio <= 1.0f)
	{
		m_fHorizontalRatio = prospectiveHorizontalRatio;
	}
	else
	{
		m_fHorizontalRatio = 1.0f;
	}
	
	UpdateGridLocation();
}

void Player::MoveSouth(float fVelocity)
{
	float prospectiveVerticalOffset =
		m_fVerticalRatio += fVelocity;

	// Don't go above the top of the screen. 
	//	Later this will be the trigger to move to the next screen.
	if (prospectiveVerticalOffset <= 1.0f)
		m_fVerticalRatio = prospectiveVerticalOffset;
	else
		m_fVerticalRatio = 1.0f;

	UpdateGridLocation();
}

void Player::MoveWest(float fVelocity)
{
	float prospectiveHorizontalOffset =
		m_fHorizontalRatio -= fVelocity;

	if (prospectiveHorizontalOffset >= 0.f)
	{
		m_fHorizontalRatio = prospectiveHorizontalOffset;
	}
	else
	{
		m_fHorizontalRatio = 0.f;
	}

	UpdateGridLocation();
}

void Player::UpdateGridLocation()
{
	int nHorizontalLocation = 
		(int)(m_fHorizontalRatio * TOTAL_GRID_DIVISIONS) /
		m_nUnitsPerGridSquare[HORIZONTAL_AXIS];

	int nVerticalLocation = 
		(int)(m_fVerticalRatio * TOTAL_GRID_DIVISIONS) /
		m_nUnitsPerGridSquare[VERTICAL_AXIS];

	m_pGridLocation[HORIZONTAL_AXIS] = nHorizontalLocation;
	m_pGridLocation[VERTICAL_AXIS] = nVerticalLocation;
}

