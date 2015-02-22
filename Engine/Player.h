#pragma once

class Player
{
public:
	Player();
	void MoveNorth(float fScreenHeight);
	void MoveEast(float fScreenWidth);
	void MoveSouth(float fScreenHeight);
	void MoveWest(float fScreenWidth);

	float GetVerticalOffset()
	{
		return m_fVerticalOffset;
	}

	float GetHorizontalOffset() 
	{
		return m_fHorizontalOffset;
	}

	void SetVerticalOffset(float verticalOffset)
	{
		m_fVerticalOffset = verticalOffset;
	}

	void SetHorizontalOffset(float horizontalOffset)
	{
		m_fHorizontalOffset = horizontalOffset;
	}

protected:

private:
	float m_fHorizontalOffset;
	float m_fVerticalOffset;
};