#pragma once

class Player
{
public:
	Player()
	{
		m_fVerticalRatio = 0.5f;
		m_fHorizontalRatio = 0.5f;
	}

	void MoveNorth();
	void MoveEast();
	void MoveSouth();
	void MoveWest();

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