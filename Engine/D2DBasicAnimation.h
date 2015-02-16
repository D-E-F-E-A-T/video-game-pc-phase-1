#pragma once

#include "DirectXBase.h"
#include "SampleOverlay.h"
#include "SimpleSprites.h"

using namespace Microsoft::WRL;

ref class D2DBasicAnimation : public DirectXBase , public Windows::ApplicationModel::Core::IFrameworkView
{
internal:
    D2DBasicAnimation();

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

    // Sample Methods
#ifdef DRAW_SPIRAL
    void CreateSpiralPathAndTriangle();
#endif // DRAW_SPIRAL
	void DrawGrid();
	void DrawPlayer();

	// Put these into XML files.
	void DrawTree();
	void DrawRock();
	void DrawDoor();
	void DrawGround();
	void DrawWater();

	void CalculateSquareCenter(int row, int column, float * x, float * y);

    float ComputeTriangleLocation(float startPoint, float endPoint, float duration, float elapsedTime);

#ifdef SHOW_OVERLAY
    SampleOverlay^                                                  m_sampleOverlay;
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
    float                                                           m_pathLength;
    float                                                           m_elapsedTime;


#ifdef SIMPLE_SPRITES
	SimpleSprites ^ m_renderer;
#endif // SIMPLE_SPRITES

	BasicSprites::SpriteBatch ^ m_spriteBatch;
	ComPtr<ID3D11Texture2D> m_asteroid;
	std::vector<AsteroidData> m_asteroidData;
};

ref class DirectXAppSource : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
    virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
