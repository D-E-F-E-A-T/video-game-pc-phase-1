#pragma once
#include "pch.h"

#include "Player.h"
#include "BaseSpriteData.h"
#include "GridSpace.h"
#include "BasicSprites.h"
#include <list>


class NarrowCollisionStrategy
{
public:
	NarrowCollisionStrategy();
	~NarrowCollisionStrategy();

	bool Detect(
		ID3D11DeviceContext2 * context,
		ID3D11Device2 * device,
		ID3D11Texture2D * texture1,
		ID3D11Texture2D * texture2,
		Player * pPlayer,
		std::list<BaseSpriteData *> * sprites,
		float * playerLocation,
		Grid * grid);

protected:
private:

	uint32_t * readPixels(
		ID3D11DeviceContext2 * context,
		ID3D11Device2 * device,
		ID3D11Texture2D * texture,
		int * width,
		int * height);

	RECT IntersectRect(
		int * playerTopLeft,
		int * obstacleTopLeft,
		int width,
		int height);
};