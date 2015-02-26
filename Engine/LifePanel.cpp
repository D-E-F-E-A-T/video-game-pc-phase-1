#include "pch.h"
#include "LifePanel.h"
#include "HeartData.h"
#include "Constants.h"

LifePanel::LifePanel(
	float fHorizontalOffset, 
	float fVerticalOffset, 
	float fPanelWidth, 
	float fPanelHeight)
{
	m_fHorizontalOffset = fHorizontalOffset;
	m_fVerticalOffset = fVerticalOffset;
	m_fPanelWidth = fPanelWidth;
	m_fPanelHeight = fPanelHeight;
}

void LifePanel::BuildPanel(std::vector<BaseSpriteData> * m_heartData)
{
	float x = 0.0f;
	float y = 0.0f;

	m_heartData->clear();

	float fHeartPanelWidth = m_fPanelWidth * 0.9f;
	float fHeartPanelHeight = m_fPanelHeight * 0.25f;

	float fHeartPanelColumnWidth = fHeartPanelWidth / NUM_HEART_COLUMNS;
	float fHeartPanelRowHeight = fHeartPanelHeight / NUM_HEART_ROWS;

	for (int row = 0; row < 2; row++)
	{
		for (int column = 0; column < 10; column++)
		{
			HeartData data(
				0,
				0,
				m_fHorizontalOffset + fHeartPanelColumnWidth * static_cast<float>(column),
				m_fVerticalOffset + fHeartPanelRowHeight * static_cast<float>(row));

			m_heartData->push_back(data);
		}
	}
}