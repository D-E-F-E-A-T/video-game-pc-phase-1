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

	float fHeartPanelColumnWidth = m_fPanelWidth / NUM_HEART_COLUMNS;
	float fHeartPanelRowHeight = m_fPanelHeight / NUM_HEART_ROWS;

	for (int row = 0; row < 2; row++)
	{
		for (int column = 0; column < 10; column++)
		{
			HeartData data(
				0,
				0,
				m_fHorizontalOffset + fHeartPanelColumnWidth * static_cast<float>(column) + (fHeartPanelColumnWidth / 2.0f),
				m_fVerticalOffset + fHeartPanelRowHeight * static_cast<float>(row) + (fHeartPanelRowHeight / 2.0f));

			m_heartData->push_back(data);
		}
	}
}