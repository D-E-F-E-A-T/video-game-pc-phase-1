#pragma once
#include "pch.h"
#include "TreeData.h"
#include <vector>

class ScreenBuilder
{
public:
	ScreenBuilder(float screenWidth, float screenHeight);
	void BuildScreen(std::vector<BaseSpriteData> * m_spriteData);

protected:

private:
	float m_fScreenWidth;
	float m_fScreenHeight;

};