#pragma once

class Player
{
public:
	Player()
	{
		m_fVerticalRatio = 0.5f;
		m_fHorizontalRatio = 0.5f;
	}

	void MoveNorth(float fVelocity);
	void MoveEast(float fVelocity);
	void MoveSouth(float fVelocity);
	void MoveWest(float fVelocity);

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

protected:

private:
	float m_fHorizontalRatio;
	float m_fVerticalRatio;
};