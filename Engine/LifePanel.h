#pragma once
#include "pch.h"
#include "BaseSpriteData.h"

class LifePanel
{
public:
	LifePanel(
		float fHorizontalOffset, 
		float fVerticalOffset, 
		float fPanelWidth, 
		float fPanelHeight);

	void BuildPanel(std::vector<BaseSpriteData> * m_heartData);

protected:

private:
	float m_fHorizontalOffset;
	float m_fVerticalOffset;
	float m_fPanelWidth;
	float m_fPanelHeight;
};