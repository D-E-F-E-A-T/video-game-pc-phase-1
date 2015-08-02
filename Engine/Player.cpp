#include "pch.h"
#include "Player.h"
#include "Constants.h"


void Player::MoveNorth()
{
	float prospectiveVerticalRatio =
		m_fVerticalRatio -= PLAYER_WALKING_VELOCITY;

	// Don't go above the top of the screen. 
	//	Later this will be the trigger to move to the next screen.
	if (prospectiveVerticalRatio >= 0.f)
		m_fVerticalRatio = prospectiveVerticalRatio;
	else
		m_fVerticalRatio = 0.0f;
}

void Player::MoveWest()
{
	float prospectiveHorizontalRatio =
		m_fHorizontalRatio += PLAYER_WALKING_VELOCITY;

	if (prospectiveHorizontalRatio <= 1.0f)
	{
		m_fHorizontalRatio = prospectiveHorizontalRatio;
	}
	else
	{
		m_fHorizontalRatio = 1.0f;
	}
}

void Player::MoveSouth()
{
	float prospectiveVerticalOffset =
		m_fVerticalRatio += PLAYER_WALKING_VELOCITY;

	// Don't go above the top of the screen. 
	//	Later this will be the trigger to move to the next screen.
	if (prospectiveVerticalOffset <= 1.0f)
		m_fVerticalRatio = prospectiveVerticalOffset;
	else
		m_fVerticalRatio = 1.0f;
}

void Player::MoveEast()
{
	float prospectiveHorizontalOffset =
		m_fHorizontalRatio -= PLAYER_WALKING_VELOCITY;

	if (prospectiveHorizontalOffset >= 0.f)
	{
		m_fHorizontalRatio = prospectiveHorizontalOffset;
	}
	else
	{
		m_fHorizontalRatio = 0.f;
	}
}

