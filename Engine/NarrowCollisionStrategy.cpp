#include "pch.h"
#include "NarrowCollisionStrategy.h"
#include "Player.h"
#include "MathUtils.h"
#include <iostream>
#include "BasicSprites.h"

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
	ID3D11Texture2D * texture1,
	ID3D11Texture2D * texture2,
	Player * pPlayer,
	std::list<BaseSpriteData *> * sprites)
{


	HBITMAP	hBitmapTexture = NULL;
	HGDIOBJ hBitmap;

	ID3D11Texture2D* d3dtex = (ID3D11Texture2D*)texture1;
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

	/*
	char buf[32];
	sprintf_s(buf, "%d %d\n", desc.Width, desc.Height);
	OutputDebugStringA(buf);
	*/

	// SWAP BGR to RGB bitmap
	uint32_t *dPtr = reinterpret_cast<uint32_t*>(dptr);

	for (size_t count = 0; count < desc.Width*desc.Height * 4; count += 4) {
/*
		uint32_t t = *(dPtr);                 
		uint32_t t1 = (t & 0x00ff0000) >> 16;
		uint32_t t2 = (t & 0x000000ff) << 16;
		uint32_t t3 = (t & 0x0000ff00);
		uint32_t ta = (t & 0xFF000000);
*/

		uint32_t t = *(dPtr);
		uint32_t alpha = (t & 0xff000000) >> 24;
		uint32_t red = (t & 0x00ff0000) >> 16;
		uint32_t green = (t & 0x0000ff00) >> 8;
		uint32_t blue = (t & 0x000000ff);

		char buf[64];
		sprintf_s(buf, "%0x %0x %0x %0x\n", red, green, blue, alpha);
		OutputDebugStringA(buf);

		*(dPtr++) = red | green | blue | alpha;

	}

/*
	hBitmapTexture = CreateCompatibleBitmap(GetDC(NULL), desc.Width, desc.Height);
	SetBitmapBits(hBitmapTexture, desc.Width*desc.Height * 4, dptr);

	hBitmap = CopyImage(hBitmapTexture, IMAGE_BITMAP, desc.Width, desc.Height, LR_CREATEDIBSECTION);
*/



/*
	ID3D11Texture2D *cpuTexture = texture1;

	D3D11_TEXTURE2D_DESC cputextdesc;
	memset(&cputextdesc, 0, sizeof(cputextdesc));

	cputextdesc.Width = cputextdesc.Height = 16;
	cputextdesc.ArraySize = 4;
	cputextdesc.MipLevels = 1;
	cputextdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R32_FLOAT;
	cputextdesc.Usage = D3D11_USAGE_STAGING;
	cputextdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	cputextdesc.SampleDesc.Count = 1;
	cputextdesc.SampleDesc.Quality = 0;

	d3dDevice->CreateTexture2D(&cputextdesc, NULL, &cpuTexture);

	D3D11_MAPPED_SUBRESOURCE mres;

	d3dContext->Map(cpuTexture, NULL, D3D11_MAP_READ, 0, &mres);

	// (assert row and depth pitch are equals)
	BYTE * mappedData = reinterpret_cast<BYTE *>(mres.pData);

	for (int pixel = 0; pixel < 256; pixel++)
	{
		if (mappedData[pixel] != 0)
		{
			char buf[32];
			sprintf_s(buf, "%d %d\n", pixel, mappedData[pixel]);
			OutputDebugStringA(buf);
		}
	}

	d3dContext->Unmap(cpuTexture, NULL);
*/


/*
	ID3D11Buffer *pVBuffer;    // global

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = 16;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	d3dDevice->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE ms;
	d3dContext->Map(texture1, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
//	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                // copy the data
	d3dContext->Unmap(pVBuffer, NULL);
*/





	// http://stackoverflow.com/questions/15811389/get-byte-array-from-texture
/*
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	d3dContext->Map(
		texture2,
		0,
		D3D11_MAP_READ,
		0,
		&mappedSubresource
		);

	BYTE * mappedData = reinterpret_cast<BYTE *>(mappedSubresource.pData);
	int pitch = mappedSubresource.RowPitch;

	char buf[32];
	sprintf_s(buf, "%d\n", pitch);
	OutputDebugStringA(buf);
*/

/*
	for (int i = 0; i < 16; i++)
	{
		char buf[32];
		sprintf_s(buf, "%d = %d\n", i, mappedData[i]);
		OutputDebugStringA(buf);
	}
*/

//	*static_cast<float2*>(mappedSubresource.pData) = m_renderTargetSize;
/*
	d3dContext->Unmap(
		texture1,
		0
		);
*/

	// Get the pixels out.
	return false;
}