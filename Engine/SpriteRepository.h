#pragma once
#include "pch.h"
#include "BaseSpriteData.h"

class SpriteRepository
{
public:
	SpriteRepository();
	~SpriteRepository();

	void AddSprite(int type, BaseSpriteData * sprite);

protected:

private:
};