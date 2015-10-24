#include "pch.h"
#include "NarrowCollisionStrategy.h"
#include "Player.h"
#include "MathUtils.h"
#include <iostream>
#include "BasicSprites.h"
#include <Windows.h>

// @see http://www.gamedev.net/page/resources/_/technical/directx-and-xna/pixel-perfect-collision-detection-in-directx-r2939
// @see http://gamedev.stackexchange.com/questions/27690/reading-from-a-staging-2d-texture-array-in-directx10
// @see http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-5
// @see http://www.cleoag.ru/2013/05/12/directx-texture-hbitmap/
NarrowCollisionStrategy::NarrowCollisionStrategy()
{

}

NarrowCollisionStrategy::~NarrowCollisionStrategy()
{

}


bool NarrowCollisionStrategy::Detect(
	ID3D11DeviceContext2 * context,
	ID3D11Device2 * device,
	ID3D11Texture2D * texturePlayer,
	ID3D11Texture2D * textureTree,	// Just checking for trees, for now.
	Player * pPlayer,
	std::list<BaseSpriteData *> * collided,
	float * playerLocation,
	Grid * grid, // Player location is the coordinates of the center of the sprite.
	int * intersectRect)
{
	bool retVal = false;

	int playerDimensions[2];
	int obstacleDimensions[2];

	uint32_t * playerPixels =
		readPixels(context, device, texturePlayer, &playerDimensions[0], &playerDimensions[1]);

	uint32_t * obstaclePixels =
		readPixels(context, device, textureTree, &obstacleDimensions[0], &obstacleDimensions[1]);

	std::list<BaseSpriteData *>::const_iterator iterator;

	int playerTopLeft[2];

	// Should really use the dimensions of the sprite.
	//	For now, using the dimensions of the grid space.
	playerTopLeft[0] = (int)playerLocation[0] - grid->GetColumnWidth() / 2;
	playerTopLeft[1] = (int)playerLocation[1] - grid->GetRowHeight() / 2;

	for (iterator = collided->begin(); iterator != collided->end(); iterator++)
	{
		float obstacleLocation[2];

		obstacleLocation[0] = (*iterator)->pos.x;
		obstacleLocation[1] = (*iterator)->pos.y;

		// Right now, all obstacles are assumed to occupy exactly one grid space.
		int obstacleTopLeft[2];
		obstacleTopLeft[0] = (int)obstacleLocation[0] - grid->GetColumnWidth() / 2;
		obstacleTopLeft[1] = (int)obstacleLocation[1] - grid->GetRowHeight() / 2;

//		int intersection[4];
		retVal = IntersectRect(
			playerTopLeft, 
			obstacleTopLeft, 
			(int)grid->GetColumnWidth(), 
			(int)grid->GetRowHeight(), 
			intersectRect);

		if (retVal == true)
		{
			return true;
		}


		char buf[128];
		sprintf_s(buf,
			"pX=%d pY=%d oX=%d oY=%d width=%f height=%f %d %d %d %d\n",
			playerTopLeft[0],
			playerTopLeft[1],
			obstacleTopLeft[0],
			obstacleTopLeft[1],
			grid->GetColumnWidth(),
			grid->GetRowHeight(),
			intersectRect[0],
			intersectRect[1],
			intersectRect[2],
			intersectRect[3]);
		OutputDebugStringA(buf);

		/*
				for (int row = 0; row < playerDimensions[1]; row++)
				{
					for (int column = 0; column < playerDimensions[0]; column++)
					{
		//				if ()
					}
				}
		*/
	}

	return false;;
}


uint32_t * NarrowCollisionStrategy::readPixels(
	ID3D11DeviceContext2 * context,
	ID3D11Device2 * device,
	ID3D11Texture2D * texture,
	int * width,
	int * height)
{
	HBITMAP	hBitmapTexture = NULL;
	HGDIOBJ hBitmap;

	ID3D11Texture2D* d3dtex = (ID3D11Texture2D*)texture;
	D3D11_TEXTURE2D_DESC desc;
	d3dtex->GetDesc(&desc);

	ID3D11Texture2D* pNewTexture = NULL;
	D3D11_TEXTURE2D_DESC description;
	d3dtex->GetDesc(&description);

	description.BindFlags = 0;

	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	description.Usage = D3D11_USAGE_STAGING;
	HRESULT hr = device->CreateTexture2D(&description, NULL, &pNewTexture);

	ID3D11DeviceContext* ctx = NULL;
	device->GetImmediateContext(&ctx);

	ctx->CopyResource(pNewTexture, d3dtex);

	D3D11_MAPPED_SUBRESOURCE resource;
	UINT subresource = D3D11CalcSubresource(0, 0, 0);
	ctx->Map(pNewTexture, subresource, D3D11_MAP_READ_WRITE, 0, &resource);

	// COPY from texture to bitmap buffer
	uint8_t* sptr = reinterpret_cast<uint8_t*>(resource.pData);
	uint8_t* dptr = new uint8_t[desc.Width*desc.Height * 4];

	for (size_t h = 0; h < desc.Height; ++h)
	{
		size_t msize = std::min<size_t>(desc.Width * 4, resource.RowPitch);
		memcpy_s(dptr, desc.Width * 4, sptr, msize);
		sptr += resource.RowPitch;
		dptr += desc.Width * 4;
	}

	dptr -= desc.Width*desc.Height * 4;

	// SWAP BGR to RGB bitmap
	uint32_t *dPtr = reinterpret_cast<uint32_t*>(dptr);

	for (size_t count = 0; count < desc.Width*desc.Height * 4; count += 4)
	{
		uint32_t t = *(dPtr);
		uint32_t alpha = (t & 0xff000000) >> 24;
		uint32_t red = (t & 0x00ff0000) >> 16;
		uint32_t green = (t & 0x0000ff00) >> 8;
		uint32_t blue = (t & 0x000000ff);

		/*
				char buf[64];
				sprintf_s(buf, "%0x %0x %0x %0x\n", red, green, blue, alpha);
				OutputDebugStringA(buf);
		*/

		/*
				if (red > 0 || green > 0 || blue > 0)
				{
					*(dPtr++) = 0xffffffff;
				}
				else
				{
					*(dPtr++) = 0x00000000;
				}
		*/
		*(dPtr++) = red | green | blue | alpha;
	}

	/*
		hBitmapTexture = CreateCompatibleBitmap(GetDC(NULL), desc.Width, desc.Height);
		SetBitmapBits(hBitmapTexture, desc.Width*desc.Height * 4, dptr);

		hBitmap = CopyImage(hBitmapTexture, IMAGE_BITMAP, desc.Width, desc.Height, LR_CREATEDIBSECTION);
	*/
	*width = desc.Width;
	*height = desc.Height;

	return dPtr;
}


// Project the coordinates of each rectangle to the
//	x and y axes. The second and third values will be 
//	the intersection.
bool NarrowCollisionStrategy::IntersectRect(
	int * playerTopLeft,
	int * obstacleTopLeft,
	int width,
	int height,
	int * retVal)
{
	int horizontalCoords[4];
	int verticalCoords[4];

	// Check if there is any intersection.
	bool horizontalOverlap = true;
	bool verticalOverlap = true;

	horizontalCoords[0] = playerTopLeft[0];
	horizontalCoords[1] = playerTopLeft[0] + width;
	horizontalCoords[2] = obstacleTopLeft[0];
	horizontalCoords[3] = obstacleTopLeft[0] + width;

	verticalCoords[0] = playerTopLeft[1];
	verticalCoords[1] = playerTopLeft[1] + height;
	verticalCoords[2] = obstacleTopLeft[1];
	verticalCoords[3] = obstacleTopLeft[1] + height;

	if (horizontalCoords[0] <= horizontalCoords[2])
	{
		if (horizontalCoords[1] < horizontalCoords[2])
		{
			horizontalOverlap = false;
		}
	}

	if (horizontalOverlap == true)
	{
		if (horizontalCoords[2] <= horizontalCoords[0])
		{
			if (horizontalCoords[3] < horizontalCoords[0])
			{
				horizontalOverlap = false;
			}
		}
	}

	if (verticalCoords[0] <= verticalCoords[2])
	{
		if (verticalCoords[1] < verticalCoords[2])
		{
			verticalOverlap = false;
		}
	}

	if (verticalOverlap == true)
	{
		if (verticalCoords[2] <= verticalCoords[0])
		{
			if (verticalCoords[3] < verticalCoords[0])
			{
				verticalOverlap = false;
			}
		}
	}


	if (verticalOverlap == true && horizontalOverlap == true)
	{
		//horizontalCoords[0] = playerTopLeft[0];
		//horizontalCoords[1] = playerTopLeft[0] + width;
		//horizontalCoords[2] = obstacleTopLeft[0];
		//horizontalCoords[3] = obstacleTopLeft[0] + width;

		InsertionSort(horizontalCoords, 4);

		//verticalCoords[0] = playerTopLeft[1];
		//verticalCoords[1] = playerTopLeft[1] + height;
		//verticalCoords[2] = obstacleTopLeft[1];
		//verticalCoords[3] = obstacleTopLeft[1] + height;

		InsertionSort(verticalCoords, 4);

		retVal[0] = horizontalCoords[1];
		retVal[1] = horizontalCoords[2];

		retVal[2] = verticalCoords[1];
		retVal[3] = verticalCoords[2];

		return true;
	}

	return false;
}

// http://cforbeginners.com/insertionsort.html
void NarrowCollisionStrategy::InsertionSort(int values [], int length)
{
	int j, temp;

	for (int i = 0; i < length; i++) {
		j = i;

		while (j > 0 && values[j] < values[j - 1]) {
			temp = values[j];
			values[j] = values[j - 1];
			values[j - 1] = temp;
			j--;
		}
	}
}
