#include "pch.h"
#include "Player.h"
#include "Constants.h"

Player::Player()
{

}

void Player::MoveNorth(float fScreenHeight)
{
	float prospectiveVerticalOffset =
		m_fVerticalOffset -= PLAYER_WALKING_VELOCITY;

	// Don't go above the top of the screen. 
	//	Later this will be the trigger to move to the next screen.
	if (prospectiveVerticalOffset >= 0.f)
		m_fVerticalOffset = prospectiveVerticalOffset;
	else
		m_fVerticalOffset = 0.0f;
}

void Player::MoveWest(float fScreenWidth)
{
	float prospectiveHorizontalOffset =
		m_fHorizontalOffset += PLAYER_WALKING_VELOCITY;

	if (prospectiveHorizontalOffset <=
		fScreenWidth - (fScreenWidth * RIGHT_MARGIN_RATIO))
	{
		m_fHorizontalOffset = prospectiveHorizontalOffset;
	}
	else
	{
		m_fHorizontalOffset =
			fScreenWidth - (fScreenWidth * RIGHT_MARGIN_RATIO);
	}
}

void Player::MoveSouth(float fScreenHeight)
{
	float prospectiveVerticalOffset =
		m_fVerticalOffset += PLAYER_WALKING_VELOCITY;

	// Don't go above the top of the screen. 
	//	Later this will be the trigger to move to the next screen.
	if (prospectiveVerticalOffset <= fScreenHeight)
		m_fVerticalOffset = prospectiveVerticalOffset;
	else
		m_fVerticalOffset = fScreenHeight;
}

void Player::MoveEast(float fScreenWidth)
{
	float prospectiveHorizontalOffset =
		m_fHorizontalOffset -= PLAYER_WALKING_VELOCITY;

	if (prospectiveHorizontalOffset >= (fScreenWidth * LEFT_MARGIN_RATIO))
	{
		m_fHorizontalOffset = prospectiveHorizontalOffset;
	}
	else
	{
		m_fHorizontalOffset = fScreenWidth * LEFT_MARGIN_RATIO;
	}
}

