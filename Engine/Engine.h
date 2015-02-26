#pragma once

#include "pch.h"
#include "Constants.h"
#include "DirectXBase.h"
#include "SampleOverlay.h"
#include "SimpleSprites.h"
#include "DebugOverlay.h"
#include "CollisionDetectionStrategy.h"
#include "ScreenBuilder.h"
#include "Player.h"

using namespace Microsoft::WRL;

ref class Engine : public DirectXBase , public Windows::ApplicationModel::Core::IFrameworkView
{
internal:
    Engine();

    // DirectXBase Methods
    virtual void CreateDeviceIndependentResources() override;
    virtual void CreateDeviceResources() override;
    virtual void Render() override;
	void Update(float timeTotal, float timeDelta);
	virtual void CreateWindowSizeDependentResources() override;

public:
    // IFrameworkView Methods
    virtual void Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
    virtual void SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);
    virtual void Load(_In_ Platform::String^ entryPoint);
    virtual void Run();
    virtual void Uninitialize();

private:
    // Event Handlers
    void OnWindowSizeChanged(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
        );

    void OnVisibilityChanged(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::VisibilityChangedEventArgs^ args
        );

    void OnDpiChanged(_In_ Windows::Graphics::Display::DisplayInformation^ sender, _In_ Platform::Object^ args);

    void OnActivated(
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView,
        _In_ Windows::ApplicationModel::Activation::IActivatedEventArgs^ args
        );

    void OnSuspending(
        _In_ Platform::Object^ sender,
        _In_ Windows::ApplicationModel::SuspendingEventArgs^ args
        );

    void OnResuming(
        _In_ Platform::Object^ sender,
        _In_ Platform::Object^ args
        );

    void OnWindowClosed(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::CoreWindowEventArgs^ args
        );

	void DrawGrid();
	void DrawPlayer();
	void DrawSprites();

	void DrawLeftMargin();
	void DrawRightMargin();

	void RenderControllerInput();

#ifdef SHOW_OVERLAY
    SampleOverlay^                                                  m_sampleOverlay;
	DebugOverlay ^	m_debugOverlay;
#endif // SHOW_OVERLAY

    bool                                                            m_windowClosed;
    bool                                                            m_windowVisible;
    ComPtr<ID2D1SolidColorBrush>                    m_blackBrush;
    ComPtr<IDWriteTextFormat>                       m_textFormat;
    ComPtr<ID2D1PathGeometry>                       m_pathGeometry;
    ComPtr<ID2D1PathGeometry>                       m_objectGeometry;
    ComPtr<ID2D1SolidColorBrush>                    m_whiteBrush;
	ComPtr<ID2D1SolidColorBrush>					m_orangeBrush;
	ComPtr<ID2D1SolidColorBrush>					m_greenBrush;
	ComPtr<ID2D1SolidColorBrush>					m_grayBrush;
	ComPtr<ID2D1SolidColorBrush>					m_beigeBrush;
	ComPtr<ID2D1SolidColorBrush>					m_blueBrush;
	ComPtr<ID2D1SolidColorBrush>					m_redBrush;
    float                                                           m_pathLength;
    float                                                           m_elapsedTime;

	void DrawHeader(const wchar_t* pText, const D2D1_RECT_F& loc);
	void DrawText(const wchar_t* pText, const D2D1_RECT_F& loc);
	void DrawText(uint32 value, const D2D1_RECT_F& loc);
	void DrawText(int16 value, const D2D1_RECT_F& loc);
	void DrawText(uint8 value, const D2D1_RECT_F& loc);
	void DrawButtonText(uint16 buttons, const D2D1_RECT_F& loc);

#ifdef SIMPLE_SPRITES
	SimpleSprites ^ m_renderer;
#endif // SIMPLE_SPRITES

	BasicSprites::SpriteBatch ^ m_spriteBatch;

	ComPtr<ID3D11Texture2D> m_tree;
	std::vector<BaseSpriteData> m_treeData;

	ComPtr<ID3D11Texture2D> m_rock;
	std::vector<BaseSpriteData> m_rockData;

	ComPtr<ID3D11Texture2D> m_water;
	std::vector<BaseSpriteData> m_waterData;

	ComPtr<ID3D11Texture2D> m_stoneWall;
	std::vector<BaseSpriteData> m_stoneWallData;

	ComPtr<ID3D11Texture2D> m_grass;
	std::vector<BaseSpriteData> m_grassData;

	ComPtr<ID3D11Texture2D> m_orchi;
	OrchiData m_orchiData;

	ComPtr<ID3D11Texture2D> m_heart;
	std::vector<BaseSpriteData> m_heartData;

	// Input related members
	bool                    m_isControllerConnected;  // Do we have a controller connected
	XINPUT_CAPABILITIES     m_xinputCaps;             // Capabilites of the controller
	XINPUT_STATE            m_xinputState;            // The current state of the controller
	uint64                  m_lastEnumTime;           // Last time a new controller connection was checked

	Microsoft::WRL::ComPtr<IDWriteTextFormat>       m_headerTextFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>       m_dataTextFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_textBrush;



//	float m_fCurrentPlayerVerticalOffset;
//	float m_fCurrentPlayerHorizontalOffset;

	int m_nCollidedSpriteColumn;
	int m_nCollidedSpriteRow;

	bool m_bSpriteCollisionDetected;

	CollisionDetectionStrategy * m_collisionDetectionStrategy;
	ScreenBuilder * m_screenBuilder;


	void SetupScreen();
	void FetchControllerInput();
	int FetchKeyboardInput();

	void CreateLifeText();
	void DrawLifeText();

	void MovePlayer(uint16 buttons);

	void HighlightSprite(int column, int row);
	// void HighLightCollidedSprite(ISpriteData sprite);	// TODO	
	
	void DisplaySpriteCollisionMessage(int column, int row);

	Player * m_pPlayer;

	Microsoft::WRL::ComPtr<IDWriteTextLayout1> m_textLayout1;
	DWRITE_TEXT_RANGE m_textRange;
};

ref class DirectXAppSource : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
    virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
