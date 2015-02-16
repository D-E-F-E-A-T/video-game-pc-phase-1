#include "pch.h"
#include "D2DBasicAnimation.h"
#include "Constants.h"
#include "Tree.h"
#include "Door.h"
#include "Ground.h"
#include "Rock.h"
#include "Water.h"
#include "BasicTimer.h"

using namespace Microsoft::WRL;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

static const float AnimationDuration = 20.0f; // Defines how long it takes the triangle to traverse the path.

D2DBasicAnimation::D2DBasicAnimation() :
    m_windowClosed(false),
    m_windowVisible(true),
    m_pathLength(0.0f),
    m_elapsedTime(0.0f)
{
}

void D2DBasicAnimation::CreateDeviceIndependentResources()
{
    DirectXBase::CreateDeviceIndependentResources();
}

#ifdef DRAW_SPIRAL
void D2DBasicAnimation::CreateSpiralPathAndTriangle()
{
    DX::ThrowIfFailed(
        m_d2dFactory->CreatePathGeometry(&m_pathGeometry)
        );

    {
        ComPtr<ID2D1GeometrySink> geometrySink;

        // Write to the path geometry using the geometry sink. We are going to create a
        // spiral.
        DX::ThrowIfFailed(
            m_pathGeometry->Open(&geometrySink)
            );

        D2D1_POINT_2F currentLocation = {0, 0};

        geometrySink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_FILLED);

        D2D1_POINT_2F locDelta = {2, 2};
        float radius = 3;

        for (UINT i = 0; i < 30; ++i)
        {
            currentLocation.x += radius * locDelta.x;
            currentLocation.y += radius * locDelta.y;

            geometrySink->AddArc(
                D2D1::ArcSegment(
                    currentLocation,
                    D2D1::SizeF(2*radius, 2*radius),    // radius x/y
                    0.0f,                               // rotation angle
                    D2D1_SWEEP_DIRECTION_CLOCKWISE,
                    D2D1_ARC_SIZE_SMALL
                    )
                );

            locDelta = D2D1::Point2F(-locDelta.y, locDelta.x);

            radius += 3;
        }

        geometrySink->EndFigure(D2D1_FIGURE_END_OPEN);

        DX::ThrowIfFailed(
            geometrySink->Close()
            );
    }

    DX::ThrowIfFailed(
        m_d2dFactory->CreatePathGeometry(&m_objectGeometry)
        );

    {
        ComPtr<ID2D1GeometrySink> geometrySink;

        // Create a simple triangle by writing the object geometry using the geometry sink.
        DX::ThrowIfFailed(
            m_objectGeometry->Open(&geometrySink)
            );

        geometrySink->BeginFigure(
            D2D1::Point2F(0.0f, 0.0f),
            D2D1_FIGURE_BEGIN_FILLED
            );

        const D2D1_POINT_2F triangle[] = {{-10.0f, -10.0f}, {-10.0f, 10.0f}, {0.0f, 0.0f}};
        geometrySink->AddLines(triangle, 3);

        geometrySink->EndFigure(D2D1_FIGURE_END_OPEN);

        DX::ThrowIfFailed(
            geometrySink->Close()
            );
    }

    DX::ThrowIfFailed(
        m_pathGeometry->ComputeLength(
            nullptr,            // Apply no transform.
            &m_pathLength
            )
        );
}
#endif // DRAW_SPIRAL

float D2DBasicAnimation::ComputeTriangleLocation(float startPoint, float endPoint, float duration, float elapsedTime)
{
    float time = min(max(elapsedTime, 0), duration);
    return startPoint + ((endPoint - startPoint) * (time / duration));
}

void D2DBasicAnimation::CreateDeviceResources()
{
    DirectXBase::CreateDeviceResources();

#ifdef DRAW_SPIRAL
    this->CreateSpiralPathAndTriangle();
#endif // DRAW_SPIRAL

#ifdef SHOW_OVERLAY
    m_sampleOverlay = ref new SampleOverlay();

    m_sampleOverlay->Initialize(
        m_d2dDevice.Get(),
        m_d2dContext.Get(),
        m_wicFactory.Get(),
        m_dwriteFactory.Get(),
        "Direct2D animation sample"
        );
#endif // SHOW_OVERLAY

    DX::ThrowIfFailed(
        m_d2dContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_blackBrush
            )
        );

    DX::ThrowIfFailed(
        m_d2dContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &m_whiteBrush
            )
        );

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Orange),
		&m_orangeBrush
			)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&m_greenBrush)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Gray),
		&m_grayBrush)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Beige),
		&m_beigeBrush)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::SkyBlue),
		&m_blueBrush)
		);
}


void D2DBasicAnimation::Render()
{

    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = m_d2dContext->GetSize();

    m_d2dContext->BeginDraw();

    m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
    m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

	DrawGrid();
	DrawPlayer();

	// Could use factories. Pass in the screen ID, row, column.
	DrawTree();
	DrawRock();
	DrawDoor();
	DrawGround();
	DrawWater();

    float minWidthHeightScale = min(renderTargetSize.width, renderTargetSize.height) / 512;

    D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(
        minWidthHeightScale,
        minWidthHeightScale
        );

    D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(
        renderTargetSize.width / 2.0f,
        renderTargetSize.height / 2.0f
        );

    // Center the path.
    m_d2dContext->SetTransform(scale * translation);

    // Draw the path in black.
#ifdef DRAW_SPIRAL
    m_d2dContext->DrawGeometry(m_pathGeometry.Get(), m_blackBrush.Get());
#endif // DRAW_SPIRAL

    float length = ComputeTriangleLocation(0.0f, m_pathLength, AnimationDuration, m_elapsedTime);

    // Ask the geometry to give us the point that corresponds with the
    // length at the current time.
#ifdef DRAW_SPIRAL
    D2D1_POINT_2F point;
    D2D1_POINT_2F tangent;

    DX::ThrowIfFailed(
        m_pathGeometry->ComputePointAtLength(
            length,
            nullptr,
            &point,
            &tangent
            )
        );

    // Reorient the triangle so that it follows the
    // direction of the path.
    D2D1_MATRIX_3X2_F triangleMatrix;
    triangleMatrix = D2D1::Matrix3x2F(
        tangent.x, tangent.y,
        -tangent.y, tangent.x,
        point.x, point.y
        );

    m_d2dContext->SetTransform(triangleMatrix * scale * translation);

    // Draw the white triangle.
    m_d2dContext->FillGeometry(m_objectGeometry.Get(), m_whiteBrush.Get());
#endif // DRAW_SPIRAL

    // When we reach the end of the animation, loop back to the beginning.
    if (m_elapsedTime >= AnimationDuration)
    {
        m_elapsedTime = 0.0f;
    }
    else
    {
        m_elapsedTime += 0.0166f; // This controls the number of time units that time is incremented by on every render call.
    }

    // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
    // is lost. It will be handled during the next call to Present.
    HRESULT hr = m_d2dContext->EndDraw();
    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }

#ifdef SHOW_OVERLAY
    m_sampleOverlay->Render();
#endif // SHOW_OVERLAY
}

void D2DBasicAnimation::Initialize(
    _In_ CoreApplicationView^ applicationView
    )
{
    applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &D2DBasicAnimation::OnActivated);

    CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &D2DBasicAnimation::OnSuspending);

    CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &D2DBasicAnimation::OnResuming);


#ifdef SIMPLE_SPRITES
	m_renderer = ref new SimpleSprites();
#endif // SIMPLE_SPRITES
}

void D2DBasicAnimation::SetWindow(
    _In_ CoreWindow^ window
    )
{
    window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

    window->SizeChanged +=
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &D2DBasicAnimation::OnWindowSizeChanged);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &D2DBasicAnimation::OnVisibilityChanged);

    window->Closed +=
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &D2DBasicAnimation::OnWindowClosed);

    DisplayInformation::GetForCurrentView()->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &D2DBasicAnimation::OnDpiChanged);

    // Disable all pointer visual feedback for better performance when touching.
    auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
    pointerVisualizationSettings->IsContactFeedbackEnabled = false;
    pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;

#ifdef SIMPLE_SPRITES
	m_renderer->Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
#else
    DirectXBase::Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
#endif // SIMPLE_SPRITES
}

void D2DBasicAnimation::Load(
    _In_ Platform::String^ entryPoint
    )
{
}

void D2DBasicAnimation::Run()
{
//#ifdef SIMPLE_SPRITES
	BasicTimer ^ timer = ref new BasicTimer();
//#endif // SIMPLE_SPRITES

    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
//#ifdef SIMPLE_SPRITES
//			timer->Update();
//			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
//			m_renderer->Update(timer->Total, timer->Delta);
//			m_renderer->Render();
//			m_renderer->Present();
//#else // SIMPLE_SPRITES
			timer->Update();

            m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            Render();
            Present();
//#endif // SIMPLE_SPRITES:
        }
        else
        {
#ifdef SIMPLE_SPRITES
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
#else // SIMPLE_SPRITES
            m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
#endif // SIMPLE_SPRITES
        }
    }
}

void D2DBasicAnimation::Uninitialize()
{
}

void D2DBasicAnimation::OnWindowSizeChanged(
    _In_ CoreWindow^ sender,
    _In_ WindowSizeChangedEventArgs^ args
    )
{
    UpdateForWindowSizeChange();

#ifdef SHOW_OVERLAY
    m_sampleOverlay->UpdateForWindowSizeChange();
#endif // SHOW_OVERLAY
}

void D2DBasicAnimation::OnVisibilityChanged(
    _In_ CoreWindow^ sender,
    _In_ VisibilityChangedEventArgs^ args
    )
{
    m_windowVisible = args->Visible;
}

void D2DBasicAnimation::OnWindowClosed(
    _In_ CoreWindow^ sender,
    _In_ CoreWindowEventArgs^ args
    )
{
    m_windowClosed = true;
}

void D2DBasicAnimation::OnDpiChanged(_In_ DisplayInformation^ sender, _In_ Platform::Object^ args)
{
    SetDpi(sender->LogicalDpi);
}

void D2DBasicAnimation::OnActivated(
    _In_ CoreApplicationView^ applicationView,
    _In_ IActivatedEventArgs^ args
    )
{
	CoreWindow::GetForCurrentThread()->Activate();
//     m_window->Activate();
}

void D2DBasicAnimation::OnSuspending(
    _In_ Platform::Object^ sender,
    _In_ SuspendingEventArgs^ args
    )
{
    // Hint to the driver that the app is entering an idle state and that its memory
    // can be temporarily used for other apps.
    Trim();
}

void D2DBasicAnimation::OnResuming(
    _In_ Platform::Object^ sender,
    _In_ Platform::Object^ args
    )
{
}

void D2DBasicAnimation::DrawPlayer()
{
	float x = 0.0f;
	float y = 0.0f;

	CalculateSquareCenter(5, 5, &x, &y);

	D2D1_RECT_F rect
	{
		x - 10.0f,
		y - 10.0f,
		x + 10.0f,
		y + 10.0f
	};

	m_d2dContext->DrawRectangle(
		rect,
		m_orangeBrush.Get());
}

void D2DBasicAnimation::DrawGrid()
{
	float windowWidth = m_window->Bounds.Width;
	float windowHeight = m_window->Bounds.Height;

	// TODO: Use arrays instead of separate variables.
	float rowHeight = (windowHeight - 2.0f * MARGIN) / NUM_GRID_ROWS;
	float columnWidth = (windowWidth - 2.0f * MARGIN) / NUM_GRID_COLUMNS;

	// Draw the horizontal lines.
	for (int row = 0; row <= NUM_GRID_ROWS; row++)
	{
		D2D1_POINT_2F src
		{ 
			MARGIN, 
			MARGIN + (rowHeight * (float) row)
		};

		D2D1_POINT_2F dst
		{ 
			windowWidth - MARGIN, 
			MARGIN + (rowHeight * (float) row)
		};

		m_d2dContext->DrawLine(src, dst, m_orangeBrush.Get());
	}

	// Draw the vertical lines.
	for (int column = 0; column <= NUM_GRID_COLUMNS; column++)
	{
		D2D1_POINT_2F src
		{
			MARGIN + (columnWidth * (float) column),
			MARGIN
		};

		D2D1_POINT_2F dst
		{
			MARGIN + (columnWidth * (float) column),
			windowHeight - MARGIN,
		};

		m_d2dContext->DrawLine(src, dst, m_orangeBrush.Get());
	}
}

void D2DBasicAnimation::DrawTree()
{
	float x = 0.0f;
	float y = 0.0f;

	CalculateSquareCenter(0, 0, &x, &y);

	Tree tree{ 0, 0, x, y, m_greenBrush };
	tree.Draw(m_d2dContext);
}

void D2DBasicAnimation::DrawRock()
{
	float x = 0.0f;
	float y = 0.0f;

	CalculateSquareCenter(1, 1, &x, &y);

	Rock rock{ 0, 0, x, y, m_grayBrush };
	rock.Draw(m_d2dContext);
}

void D2DBasicAnimation::DrawDoor()
{
	float x = 0.0f;
	float y = 0.0f;

	CalculateSquareCenter(2, 2, &x, &y);

	Door door{ 0, 0, x, y, m_blackBrush };
	door.Draw(m_d2dContext);
}

void D2DBasicAnimation::DrawGround()
{
	float x = 0.0f;
	float y = 0.0f;

	CalculateSquareCenter(3, 3, &x, &y);

	Ground ground{ 0, 0, x, y, m_beigeBrush };
	ground.Draw(m_d2dContext);
}

void D2DBasicAnimation::DrawWater()
{
	float x = 0.0f;
	float y = 0.0f;

	CalculateSquareCenter(8, 8, &x, &y);

	Water water{ 0, 0, x, y, m_blueBrush };
	water.Draw(m_d2dContext);
}

void D2DBasicAnimation::CalculateSquareCenter(int row, int column, float * x, float * y)
{
	float windowWidth = m_window->Bounds.Width;
	float windowHeight = m_window->Bounds.Height;

	float rowHeight = (windowHeight - 2.0f * MARGIN) / NUM_GRID_ROWS;
	float columnWidth = (windowWidth - 2.0f * MARGIN) / NUM_GRID_COLUMNS;

	*x = MARGIN + (columnWidth * column) + (columnWidth / 2.0f);
	*y = MARGIN + (rowHeight * row) + (rowHeight / 2.0f);
}

IFrameworkView^ DirectXAppSource::CreateView()
{
    return ref new D2DBasicAnimation();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto directXAppSource = ref new DirectXAppSource();
    CoreApplication::Run(directXAppSource);
    return 0;
}
