#pragma once
#include "Grid.h"
#include "Constants.h"

class Player
{
public:
	Player(Grid * grid);

	void MoveNorth(int nCollisionState, float fVelocity);
	void MoveEast(int nCollisionState, float fVelocity);
	void MoveSouth(int nCollisionState, float fVelocity);
	void MoveWest(int nCollisionState, float fVelocity);

	float GetVerticalRatio()
	{
		return m_fVerticalRatio;
	}

	float GetHorizontalRatio() 
	{
		return m_fHorizontalRatio;
	}

	void SetVerticalRatio(float verticalOffset)
	{
		m_fVerticalRatio = verticalOffset;
	}

	void SetHorizontalRatio(float horizontalOffset)
	{
		m_fHorizontalRatio = horizontalOffset;
	}

	// Grid square where the player is currently located.
	int * GetGridLocation()
	{
		return m_pGridLocation;
	}

protected:
	void UpdateGridLocation();

private:
	float m_fHorizontalRatio;
	float m_fVerticalRatio;

	float m_fLocation[NUM_DIMENSIONS];
	int m_pGridLocation[NUM_DIMENSIONS];

	int m_nUnitsPerGridSquare[NUM_DIMENSIONS];

	Grid * m_grid;

	int m_nPreviousMoveDirection;
};