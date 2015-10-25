#include "pch.h"
#include "Player.h"
#include "Constants.h"

Player::Player(Grid * grid)
{
	m_grid = grid;
	m_fHorizontalRatio = 0.5f;
	m_fVerticalRatio = 0.5f;
	m_nPreviousMoveDirection = CENTER;

	m_nUnitsPerGridSquare[HORIZONTAL_AXIS] =
		1.0f / grid->GetNumColumns() * TOTAL_GRID_DIVISIONS;

	m_nUnitsPerGridSquare[VERTICAL_AXIS] =
		1.0f / grid->GetNumRows() * TOTAL_GRID_DIVISIONS;

	UpdateGridLocation();
}

void Player::MoveNorth(int nCollisionState, float fVelocity)
{
	if (nCollisionState == COLLISION && m_nPreviousMoveDirection == NORTH)
	{
	}
	else
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

	m_nPreviousMoveDirection = NORTH;
}

void Player::MoveEast(int nCollisionState, float fVelocity)
{
	if (nCollisionState == COLLISION && m_nPreviousMoveDirection == EAST)
	{
	}
	else
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

	m_nPreviousMoveDirection = EAST;
}

void Player::MoveSouth(int nCollisionState, float fVelocity)
{
	if (nCollisionState == COLLISION && m_nPreviousMoveDirection == SOUTH)
	{
	}
	else
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

	m_nPreviousMoveDirection = SOUTH;
}

void Player::MoveWest(int nCollisionState, float fVelocity)
{
	if (nCollisionState == COLLISION && m_nPreviousMoveDirection == WEST)
	{
	}
	else
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

	m_nPreviousMoveDirection = WEST;
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

