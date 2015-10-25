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
	bool bIntersection = false;

	int rawPlayerDimensions[2];
	int rawObstacleDimensions[2];

	uint8_t * playerPixels = NULL;
	uint8_t * obstaclePixels = NULL;
/*
	playerPixels = readPixels(
		context,
		device,
		texturePlayer,
		rawPlayerDimensions);	// These are the dimensions of the raw sprite.
*/


	obstaclePixels = readPixels(
		context,
		device,
		textureTree,
		rawObstacleDimensions);	// These are the dimensions of the raw sprite.

	for (size_t count = 0; count < rawObstacleDimensions[0] * rawObstacleDimensions[1] * 4; count += 4)
	{
		uint32_t t = *(obstaclePixels);
		uint32_t alpha = (t & 0xff000000) >> 24;
		uint32_t red = (t & 0x00ff0000) >> 16;
		uint32_t green = (t & 0x0000ff00) >> 8;
		uint32_t blue = (t & 0x000000ff);


		char buf[64];
		sprintf_s(buf, "%02x %02x %02x %02x\n", red, green, blue, alpha);
		OutputDebugStringA(buf);


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
//		*(dPtr++) = red | green | blue | alpha;

		//		size++;
	}



/*
	std::list<BaseSpriteData *>::const_iterator iterator;

	int playerTopLeft[2];

	// Should really use the dimensions of the sprite.
	//	For now, using the dimensions of the grid space.
	playerTopLeft[HORIZONTAL_AXIS] = (int)playerLocation[HORIZONTAL_AXIS] - grid->GetColumnWidth() / 2;
	playerTopLeft[VERTICAL_AXIS] = (int)playerLocation[VERTICAL_AXIS] - grid->GetRowHeight() / 2;

	for (iterator = collided->begin(); iterator != collided->end(); iterator++)
	{
		int renderedSpriteDimensions[2];
		float obstacleCenterLocation[2];

		obstacleCenterLocation[HORIZONTAL_AXIS] = (*iterator)->pos.x;
		obstacleCenterLocation[VERTICAL_AXIS] = (*iterator)->pos.y;

		// These are relative to the rendered sprite.
		//	Take into consideration the actual screen dimensions.
		renderedSpriteDimensions[WIDTH_INDEX] = (int)grid->GetColumnWidth();
		renderedSpriteDimensions[HEIGHT_INDEX] = (int)grid->GetRowHeight();



		// Right now, all obstacles are assumed to occupy exactly one grid space.
		int obstacleTopLeft[2];
		obstacleTopLeft[HORIZONTAL_AXIS] = 
			(int)obstacleCenterLocation[HORIZONTAL_AXIS] -
			renderedSpriteDimensions[WIDTH_INDEX] / 2;

		obstacleTopLeft[VERTICAL_AXIS] = 
			(int)obstacleCenterLocation[VERTICAL_AXIS] -
			renderedSpriteDimensions[HEIGHT_INDEX] / 2;

		bIntersection = IntersectRect(
			playerTopLeft,
			obstacleTopLeft,
			renderedSpriteDimensions[WIDTH_INDEX],
			renderedSpriteDimensions[HEIGHT_INDEX],
			intersectRect);

		if (bIntersection == true)
		{
			int intersectionWidth = abs(intersectRect[0] - intersectRect[1]);
			int intersectionHeight = abs(intersectRect[2] - intersectRect[3]);

			// These coordinates are relative to the whole screen.
			int playerIntersectionHorizontalOffset = intersectRect[0] - playerTopLeft[0];
			int playerIntersectionVerticalOffset = intersectRect[2] - playerTopLeft[1];

			// These coordinates are relative to the whole screen.
			int obstacleIntersectionHorizontalOffset = intersectRect[0] - obstacleTopLeft[0];
			int obstacleIntersectionVerticalOffset = intersectRect[2] - obstacleTopLeft[1];

			for (int row = 0; row < intersectionHeight; row++)
			{
				for (int column = 0; column < intersectionWidth; column++)
				{
					float playerPixelNormalizedLocation[2];
					int playerPixelRawCoordinate[2];
					float obstaclePixelNormalizedLocation[2];
					int obstaclePixelRawCoordinate[2];

					// Relative to the raw sprite dimensions (0, 1.0f)
					playerPixelNormalizedLocation[HORIZONTAL_AXIS] =
						(float)playerIntersectionHorizontalOffset / (float)renderedSpriteDimensions[HORIZONTAL_AXIS];

					// Relative to the raw sprite dimensions (0, 1.0f)
					playerPixelNormalizedLocation[VERTICAL_AXIS] =
						(float)playerIntersectionVerticalOffset / (float)renderedSpriteDimensions[VERTICAL_AXIS];

					// Relative to the raw sprite dimensions (0, 1.0f)
					obstaclePixelNormalizedLocation[HORIZONTAL_AXIS] =
						(float)obstacleIntersectionHorizontalOffset / (float)renderedSpriteDimensions[HORIZONTAL_AXIS];

					// Relative to the raw sprite dimensions (0, 1.0f)
					obstaclePixelNormalizedLocation[VERTICAL_AXIS] =
						(float)obstacleIntersectionVerticalOffset / (float)renderedSpriteDimensions[VERTICAL_AXIS];

					playerPixelRawCoordinate[HORIZONTAL_AXIS] =
						playerPixelNormalizedLocation[HORIZONTAL_AXIS] * rawPlayerDimensions[HORIZONTAL_AXIS];

					playerPixelRawCoordinate[VERTICAL_AXIS] =
						playerPixelNormalizedLocation[VERTICAL_AXIS] * rawPlayerDimensions[VERTICAL_AXIS];

					obstaclePixelRawCoordinate[HORIZONTAL_AXIS] =
						obstaclePixelNormalizedLocation[HORIZONTAL_AXIS] * rawObstacleDimensions[HORIZONTAL_AXIS];

					obstaclePixelRawCoordinate[VERTICAL_AXIS] =
						obstaclePixelNormalizedLocation[VERTICAL_AXIS] * rawObstacleDimensions[VERTICAL_AXIS];
			
					int rawPlayerPixelIndex =
						(playerPixelRawCoordinate[VERTICAL_AXIS] * rawPlayerDimensions[HORIZONTAL_AXIS]) +
						playerPixelRawCoordinate[HORIZONTAL_AXIS];

					int rawObstaclePixelIndex =
						(obstaclePixelRawCoordinate[VERTICAL_AXIS] * rawObstacleDimensions[HORIZONTAL_AXIS]) +
						obstaclePixelRawCoordinate[HORIZONTAL_AXIS];

					//uint8_t playerResult =
					//	*(playerPixels + rawPlayerPixelIndex * 4) & 0xff000000;

					//uint8_t obstacleResult =
					//	*(obstaclePixels + rawObstaclePixelIndex * 4) & 0xff000000;

					char buf[128];

					uint8_t * playerBuffer = playerPixels + rawPlayerPixelIndex * 4;

					sprintf_s(buf,
						"%x %x %x %x\n",
						*(playerBuffer),
						*(playerBuffer + 1),
						*(playerBuffer + 2),
						*(playerBuffer + 3));
					OutputDebugStringA(buf);

					//delete[] playerPixels;
					//delete[] obstaclePixels;


					//if (playerResult && obstacleResult)
					//{
					//	OutputDebugStringA("collision\n");
					//	return true;
					//}
				}
			}
		}

	}

	delete [] playerPixels;
	delete [] obstaclePixels;
*/

	return false;
}


uint8_t * NarrowCollisionStrategy::readPixels(
	ID3D11DeviceContext2 * context,
	ID3D11Device2 * device,
	ID3D11Texture2D * texture,
	int * dimensions)
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
	uint8_t* dptr = new uint8_t[desc.Width * desc.Height * 4];

	for (size_t h = 0; h < desc.Height; ++h)
	{
		size_t msize = std::min<size_t>(desc.Width * 4, resource.RowPitch);
		memcpy_s(dptr, desc.Width * 4, sptr, msize);
		sptr += resource.RowPitch;
		dptr += desc.Width * 4;
	}

	dptr -= desc.Width*desc.Height * 4;

	// SWAP BGR to RGB bitmap
	// Notice the capital "P". dptr vs dPtr.
	uint32_t * dPtr = reinterpret_cast<uint32_t*>(dptr);

//	int size = 0;

	for (size_t count = 0; count < desc.Width*desc.Height * 4; count += 4)
	{
		uint32_t t = *(dPtr);
		uint32_t alpha = (t & 0xff000000) >> 24;
		uint32_t red = (t & 0x00ff0000) >> 16;
		uint32_t green = (t & 0x0000ff00) >> 8;
		uint32_t blue = (t & 0x000000ff);


				//char buf[64];
				//sprintf_s(buf, "%02x %02x %02x %02x\n", red, green, blue, alpha);
				//OutputDebugStringA(buf);


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

//		size++;
	}

/*
	char buf[128];
	sprintf_s(buf, "num sprite pixels = %d\n", size);
	OutputDebugStringA(buf);
*/

	/*
		hBitmapTexture = CreateCompatibleBitmap(GetDC(NULL), desc.Width, desc.Height);
		SetBitmapBits(hBitmapTexture, desc.Width*desc.Height * 4, dptr);

		hBitmap = CopyImage(hBitmapTexture, IMAGE_BITMAP, desc.Width, desc.Height, LR_CREATEDIBSECTION);
	*/
	*(dimensions + WIDTH_INDEX) = desc.Width;
	*(dimensions + HEIGHT_INDEX) = desc.Height;

	return dptr;
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
		InsertionSort(horizontalCoords, 4);
		InsertionSort(verticalCoords, 4);

		retVal[0] = horizontalCoords[1];	// left
		retVal[1] = horizontalCoords[2];	// right

		retVal[2] = verticalCoords[1];		// top
		retVal[3] = verticalCoords[2];		// bottom

		return true;
	}

	return false;
}

void NarrowCollisionStrategy::InsertionSort(int values [], int length)
{
	int j, temp;

	for (int i = 0; i < length; i++) 
	{
		j = i;

		while (j > 0 && values[j] < values[j - 1]) 
		{
			temp = values[j];
			values[j] = values[j - 1];
			values[j - 1] = temp;
			j--;
		}
	}
}
