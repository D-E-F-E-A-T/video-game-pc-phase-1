#include "pch.h"
#include "ScreenBuilder.h"
#include "ScreenUtils.h"

ScreenBuilder::ScreenBuilder(float screenWidth, float screenHeight)
{
	m_fScreenWidth = screenWidth;
	m_fScreenHeight = screenHeight;
}

/*
	TODO: Use web services
*/
void ScreenBuilder::BuildScreen1(std::vector<BaseSpriteData *> * m_treeData)
{
	m_treeData->clear();

	float x = 0.0f;
	float y = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				j, i, &x, &y);

			m_treeData->push_back(new TreeData(j, i, x, y));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 11; j < 17; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				j, i, &x, &y);

			TreeData data(j, i, x, y);
			m_treeData->push_back(new TreeData(j, i, x, y));
		}
	}

	for (int i = 0; i < 5; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 4, &x, &y);
		
		m_treeData->push_back(new TreeData(i, 4, x, y));
	}

	for (int i = 0; i < 4; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 5, &x, &y);

		m_treeData->push_back(new TreeData(i, 5, x, y));
	}

	for (int i = 0; i < 3; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 6, &x, &y);
		
		m_treeData->push_back(new TreeData(i, 6, x, y));
	}


	for (int i = 12; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 4, &x, &y);

		m_treeData->push_back(new TreeData(i, 4, x, y));
	}


	for (int i = 12; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 9, &x, &y);

		m_treeData->push_back(new TreeData(i, 9, x, y));
	}

	
	for (int i = 0; i < 6; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 10, &x, &y);

		m_treeData->push_back(new TreeData(i, 10, x, y));
	}


	for (int i = 11; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 10, &x, &y);

		m_treeData->push_back(new TreeData(i, 10, x, y));
	}


	for (int i = 11; i < 17; i++)
	{
		for (int j = 11; j < 15; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				i, j, &x, &y);

			m_treeData->push_back(new TreeData(i, j, x, y));
		}
	}


	for (int i = 0; i < 7; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 11, &x, &y);

		m_treeData->push_back(new TreeData(i, 11, x, y));
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 12; j < 15; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				i, j, &x, &y);

			m_treeData->push_back(new TreeData(i, j, x, y));
		}
	}

/*
	for (int i = 0; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight,
			i, 0, &x, &y);

		m_treeData->push_back(new TreeData(i, 0, x, y));
	}
*/

/*
	ScreenUtils::CalculateSquareCenter(
		m_fScreenWidth,
		m_fScreenHeight,
		4, 4, &x, &y);

	m_treeData->push_back(new TreeData(4, 4, x, y));
*/
}