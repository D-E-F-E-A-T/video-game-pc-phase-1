//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "DirectXBase.h"
//#include "SampleOverlay.h"
//#include "AutoThrottle.h"
#include "BasicSprites.h"

namespace SampleSettings
{
    static const unsigned int NumTrees = 10;
	static const unsigned int NumRocks = 10;

//#ifdef WATER_SPRITE
	static const unsigned int NumWaters = 10;
	static const unsigned int NumGrasses = 10;
	static const unsigned int NumStoneWalls = 10;
	static const unsigned int NumLinks = 10;
//#endif // WATER_SPRITE

    namespace Performance
    {
        static const float TargetFrameTime = 1.0f / 20.0f;
        static const unsigned int InitialParticleCount = 5000;
        static const unsigned int ParticleCountDelta = 32;
        static const unsigned int ParticleCountMin = 1000;
        static const unsigned int ParticleCountMax = 60000;
    }
    namespace Physics
    {
        static const float Gravity = 40000000.0f;
        static const float Damping = 0.015f;
    }
}

struct TreeData
{
    float2 pos;
    float2 vel;
    float rot;
    float rotVel;
    float scale;
};

struct RockData
{
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
};

struct WaterData
{
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
};

struct GrassData
{
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
};

struct StoneWallData
{
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
};

struct LinkData
{
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
};

struct ParticleData
{
    float2 pos;
    float2 vel;
};

ref class SimpleSprites : public DirectXBase
{
internal:
    SimpleSprites();
    virtual void CreateDeviceIndependentResources() override;
    virtual void CreateDeviceResources() override;
    virtual void CreateWindowSizeDependentResources() override;
    virtual void Render() override;
    void Update(float timeTotal, float timeDelta);
/*
    void AddRepulsor(_In_ uint32 id, _In_ float2 position);
    void MoveRepulsor(_In_ uint32 id, _In_ float2 position);
    void RemoveRepulsor(_In_ uint32 id);
*/

private:
//    float RandFloat(float min, float max);
//    SampleOverlay^ m_sampleOverlay;
//    AutoThrottle^ m_autoThrottle;
    BasicSprites::SpriteBatch^ m_spriteBatch;
//    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tree;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rock;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_water;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_stoneWall;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_link;
//    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
    std::vector<TreeData> m_treeData;
	std::vector<RockData> m_rockData;
	std::vector<WaterData> m_waterData;
	std::vector<StoneWallData> m_stoneWallData;
	std::vector<LinkData> m_linkData;

//    std::vector<ParticleData> m_particleData;
//    std::map<uint32, float2> m_repulsors;
//    int m_numParticlesToDraw;
};
