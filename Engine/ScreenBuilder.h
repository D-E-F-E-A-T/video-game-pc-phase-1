#pragma once
#include "pch.h"
#include "TreeData.h"
#include <vector>

class ScreenBuilder
{
public:
	ScreenBuilder(float screenWidth, float screenHeight);
	void BuildScreen1(std::vector<BaseSpriteData *> * m_treeData);

protected:

private:
	float m_fScreenWidth;
	float m_fScreenHeight;

};