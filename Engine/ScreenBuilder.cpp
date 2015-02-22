#include "pch.h"
#include "ScreenBuilder.h"
#include "ScreenUtils.h"

ScreenBuilder::ScreenBuilder(float screenWidth, float screenHeight)
{
	m_fScreenWidth = screenWidth;
	m_fScreenHeight = screenHeight;
}

void ScreenBuilder::BuildScreen(std::vector<TreeData> * m_treeData)
{
	m_treeData->clear();


	float x = 0.0f;
	float y = 0.0f;

	/*
	for (int i = 0; i < 15; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight,
			i, i, &x, &y);

		TreeData data(i, i, x, y);
		m_treeData->push_back(data);
	}

	for (int i = 0; i < 15; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight,
			i, 15 - i, &x, &y);

		TreeData data(i, 15 - i, x, y);
		m_treeData->push_back(data);
	}
	*/

	/*
	ScreenUtils::CalculateSquareCenter(
		m_fScreenWidth,
		m_fScreenHeight,
		1, 13, &x, &y);

	TreeData data(1, 13, x, y);
	m_treeData->push_back(data);

	ScreenUtils::CalculateSquareCenter(
		m_fScreenWidth,
		m_fScreenHeight,
		15, 1, &x, &y);

	TreeData data1(15, 1, x, y);
	m_treeData->push_back(data1);
*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				j, i, &x, &y);

			TreeData data(j, i, x, y);
			m_treeData->push_back(data);
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
			m_treeData->push_back(data);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 4, &x, &y);
		TreeData data0(i, 4, x, y);
		m_treeData->push_back(data0);
	}

	for (int i = 0; i < 4; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 5, &x, &y);
		TreeData data0(i, 5, x, y);
		m_treeData->push_back(data0);
	}

	for (int i = 0; i < 3; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 6, &x, &y);
		TreeData data0(i, 6, x, y);
		m_treeData->push_back(data0);
	}


	for (int i = 12; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 4, &x, &y);
		TreeData data0(i, 4, x, y);
		m_treeData->push_back(data0);
	}


	for (int i = 12; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 9, &x, &y);
		TreeData data0(i, 9, x, y);
		m_treeData->push_back(data0);
	}

	
	for (int i = 0; i < 6; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 10, &x, &y);
		TreeData data(i, 10, x, y);
		m_treeData->push_back(data);
	}


	for (int i = 11; i < 17; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 10, &x, &y);
		TreeData data(i, 10, x, y);
		m_treeData->push_back(data);
	}


	for (int i = 11; i < 17; i++)
	{
		for (int j = 11; j < 15; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				i, j, &x, &y);
			TreeData data(i, j, x, y);
			m_treeData->push_back(data);
		}
	}


	for (int i = 0; i < 7; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			i, 11, &x, &y);
		TreeData data(i, 11, x, y);
		m_treeData->push_back(data);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 12; j < 15; j++)
		{
			ScreenUtils::CalculateSquareCenter(
				m_fScreenWidth,
				m_fScreenHeight,
				i, j, &x, &y);
			TreeData data(i, j, x, y);
			m_treeData->push_back(data);
		}
	}

	/*
	for (int i = 12; i < 15; i++)
	{
		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			0, i, &x, &y);
		TreeData data(i, 0, x, y);
		m_treeData->push_back(data);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			1, i, &x, &y);
		TreeData data1(i, 1, x, y);
		m_treeData->push_back(data1);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			2, i, &x, &y);
		TreeData data2(i, 2, x, y);
		m_treeData->push_back(data2);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			3, i, &x, &y);
		TreeData data3(i, 3, x, y);
		m_treeData->push_back(data3);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			4, i, &x, &y);
		TreeData data4(i, 4, x, y);
		m_treeData->push_back(data4);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			5, i, &x, &y);
		TreeData data5(i, 5, x, y);
		m_treeData->push_back(data5);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			6, i, &x, &y);
		TreeData data6(i, 6, x, y);
		m_treeData->push_back(data6);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			11, i, &x, &y);
		TreeData data11(i, 11, x, y);
		m_treeData->push_back(data11);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			12, i, &x, &y);
		TreeData data12(i, 12, x, y);
		m_treeData->push_back(data12);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			13, i, &x, &y);
		TreeData data13(i, 13, x, y);
		m_treeData->push_back(data13);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			14, i, &x, &y);
		TreeData data14(i, 14, x, y);
		m_treeData->push_back(data14);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			15, i, &x, &y);
		TreeData data15(i, 15, x, y);
		m_treeData->push_back(data15);

		ScreenUtils::CalculateSquareCenter(
			m_fScreenWidth,
			m_fScreenHeight, 
			16, i, &x, &y);
		TreeData data16(i, 16, x, y);
		m_treeData->push_back(data16);

	}
*/
}