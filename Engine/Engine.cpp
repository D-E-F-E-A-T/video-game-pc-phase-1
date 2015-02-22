#include "pch.h"
#include "Engine.h"
#include "Constants.h"
#include "Tree.h"
#include "Door.h"
#include "Ground.h"
#include "Rock.h"
#include "Water.h"
#include "BasicTimer.h"
#include "minwindef.h"
#include "BasicLoader.h"
#include "DebugOverlay.h"
#include "SimpleController.h"
#include "LeftMargin.h"
#include "RightMargin.h"
#include "TreeData.h"
#include "RockData.h"
#include "WaterData.h"
#include "GrassData.h"
#include "StoneWallData.h"
#include "OrchiData.h"
#include "BoundingBoxCollisionStrategy.h"
#include "ScreenUtils.h"

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

namespace
{
	const char16    APPLICATION_TITLE [] = L"XInput game controller sample";

	const float     CLEAR_COLOR[4] = { 0.071f, 0.040f, 0.561f, 1.0f };

	const char16    FONT_LOCAL [] = L"en-US";
	const char16    FONT_NAME [] = L"Segoe UI";
	const float     FONT_SIZE_HEADER = 18.0f;
	const float     FONT_SIZE_TEXT = 18.0f;

	const float     INFORMATION_START_X = 32.0f;
	const float     INFORMATION_START_Y = 150.0f;
	const float     LINE_HEIGHT = FONT_SIZE_HEADER * 1.5f;

	const float     INPUT_LABEL_START = INFORMATION_START_X;
	const float     INPUT_DATA_START = INFORMATION_START_X + 132.0f;
	const float     CAPS_LABEL_START = INFORMATION_START_X + 300.0f;
	const float     CAPS_DATA_START = INFORMATION_START_X + 400.0f;

	const char16    MSG_NEED_CONTROLLER [] = L"Please attach an Xbox 360 common controller device.";
	const char16	SPRITE_COLLISION_MSG [] = L"Sprite collision:";
	const char16	SPRITE_COLLISION_MSG_ROW [] = L"Row";
	const char16	SPRITE_COLLISION_MSG_COLUMN [] = L"Column";
//	const uint64    XINPUT_ENUM_TIMEOUT_MS = 2000;  // 2 seconds

	const char16    STATE_HEADER [] = L"XInput State";
	const char16    LABEL_STATE_PACKET_NUMBER [] = L"Packet Number";
	const char16    LABEL_STATE_LEFT_TRIGGER [] = L"Left Trigger";
	const char16    LABEL_STATE_RIGHT_TRIGGER [] = L"Right Trigger";
	const char16    LABEL_STATE_LEFT_THUMB_X [] = L"Left Thumb X";
	const char16    LABEL_STATE_LEFT_THUMB_Y [] = L"Left Thumb Y";
	const char16    LABEL_STATE_RIGHT_THUMB_X [] = L"Right Thumb X";
	const char16    LABEL_STATE_RIGHT_THUMB_Y [] = L"Right Thumb Y";
	const char16    LABEL_STATE_BUTTONS [] = L"Buttons";

	const char16    CAPS_HEADER [] = L"XInput Capabilties";
	const char16    LABEL_CAPS_TYPE [] = L"Type";
	const char16    LABEL_CAPS_SUBTYPE [] = L"Subtype";
	const char16    LABEL_CAPS_FLAGS [] = L"Flags";
	const char16    VALUE_CAPS_WIRED [] = L"Wired";
	const char16    VALUE_CAPS_WIRELESS [] = L"Wireless";
	const char16    VALUE_CAPS_VOICE_SUPPORT [] = L"Voice Support";
};

Engine::Engine() :
    m_windowClosed(false),
    m_windowVisible(false),
    m_pathLength(0.0f),
    m_elapsedTime(0.0f),
	m_isControllerConnected(false),
	m_nCollidedSpriteColumn(0),
	m_nCollidedSpriteRow(0)
{
	m_collisionDetectionStrategy =
		new BoundingBoxCollisionStrategy();


}

void Engine::CreateDeviceIndependentResources()
{
    DirectXBase::CreateDeviceIndependentResources();

}

void Engine::CreateDeviceResources()
{
    DirectXBase::CreateDeviceResources();

#ifdef SHOW_OVERLAY
    m_sampleOverlay = ref new SampleOverlay();

    m_sampleOverlay->Initialize(
        m_d2dDevice.Get(),
        m_d2dContext.Get(),
        m_wicFactory.Get(),
        m_dwriteFactory.Get(),
        ""
        );

	m_debugOverlay = ref new DebugOverlay();

	m_debugOverlay->Initialize(
		m_d2dDevice.Get(),
		m_d2dContext.Get(),
		m_wicFactory.Get(),
		m_dwriteFactory.Get(),
		"Debug overlay"
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

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_redBrush)
		);

	m_spriteBatch = ref new BasicSprites::SpriteBatch();
	unsigned int capacity = SampleSettings::Performance::ParticleCountMax +
		SampleSettings::NumTrees + 1;


	m_spriteBatch->Initialize(
		m_d3dDevice.Get(),
		capacity);

	BasicLoader ^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());

	loader->LoadTexture(
		"tree.dds",
		&m_tree,
		nullptr);

	m_spriteBatch->AddTexture(m_tree.Get());

	loader->LoadTexture(
		"rock.dds",
		&m_rock,
		nullptr);

	m_spriteBatch->AddTexture(m_rock.Get());

	loader->LoadTexture(
		"water.dds",
		&m_water,
		nullptr);

	m_spriteBatch->AddTexture(m_water.Get());

	loader->LoadTexture(
		"grass.dds",
		&m_grass,
		nullptr);

	m_spriteBatch->AddTexture(m_grass.Get());

	loader->LoadTexture(
		"stonewall.dds",
		&m_stoneWall,
		nullptr);

	m_spriteBatch->AddTexture(m_stoneWall.Get());

	loader->LoadTexture(
		"link.dds",
		&m_orchi,
		nullptr);

	m_spriteBatch->AddTexture(m_orchi.Get());

	//
	// Setup the local graphics objects
	//
	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		FONT_NAME,
		nullptr,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		FONT_SIZE_HEADER,
		FONT_LOCAL,
		&m_headerTextFormat
		)
		);
	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		FONT_NAME,
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		FONT_SIZE_TEXT,
		FONT_LOCAL,
		&m_dataTextFormat
		)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_textBrush)
		);
}


void Engine::Render()
{
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = m_d2dContext->GetSize();

    m_d2dContext->BeginDraw();

    m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Tan));
    m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

	DrawLeftMargin();
	DrawRightMargin();

	DrawGrid();
	DrawPlayer();

	int column = 0;
	int row = 0;

	int result = CheckForCollisions(&column, &row);
	
	if (result == 1)
	{
		// Need to undo the transform done above.
//		HighlightSprite(column, row);
		DisplaySpriteCollisionMessage(column, row);
	}

	// Could use factories. Pass in the screen ID, row, column.
	//DrawTree();
	//DrawRock();
	//DrawDoor();
	//DrawGround();
	//DrawWater();

	RenderControllerInput();

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

    // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
    // is lost. It will be handled during the next call to Present.
    HRESULT hr = m_d2dContext->EndDraw();

	m_d3dContext->OMSetRenderTargets(
		1,
		m_d3dRenderTargetView.GetAddressOf(),
		nullptr
		);

	m_spriteBatch->Begin();

	for (auto tree = m_treeData.begin(); tree != m_treeData.end(); tree++)
	{
		m_spriteBatch->Draw(
			m_tree.Get(),
			tree->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(1.0f, 1.0f) * tree->scale,
			BasicSprites::SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			tree->rot
			);
	}

	for (auto rock = m_rockData.begin(); rock != m_rockData.end(); rock++)
	{
		m_spriteBatch->Draw(
			m_rock.Get(),
			rock->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(1.0f, 1.0f) * rock->scale,
			BasicSprites::SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			rock->rot
			);
	}

//#ifdef WATER_SPRITE
	for (auto water  = m_waterData.begin(); water != m_waterData.end(); water++)
	{
		m_spriteBatch->Draw(
			m_water.Get(),
			water->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(1.0f, 1.0f) * water->scale,
			BasicSprites::SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			water->rot
			);
	}

	for (auto grass = m_grassData.begin(); grass != m_grassData.end(); grass++)
	{
		m_spriteBatch->Draw(
			m_grass.Get(),
			grass->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(1.0f, 1.0f) * grass->scale,
			BasicSprites::SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			grass->rot
			);
	}

	for (auto stoneWall = m_stoneWallData.begin(); stoneWall != m_stoneWallData.end(); stoneWall++)
	{
		m_spriteBatch->Draw(
			m_stoneWall.Get(),
			stoneWall->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(1.0f, 1.0f) * stoneWall->scale,
			BasicSprites::SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			stoneWall->rot
			);
	}

	m_spriteBatch->Draw(
		m_orchi.Get(),
		m_orchiData.pos,
		BasicSprites::PositionUnits::DIPs,
		float2(1.0f, 1.0f) * m_orchiData.scale,
		BasicSprites::SizeUnits::Normalized,
		float4(0.8f, 0.8f, 1.0f, 1.0f),
		m_orchiData.rot
		);


	m_spriteBatch->End();



    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }

#ifdef SHOW_OVERLAY
	
    m_sampleOverlay->Render();
	m_debugOverlay->Render("Hello World");
#endif // SHOW_OVERLAY
}

void Engine::Initialize(
    _In_ CoreApplicationView^ applicationView
    )
{
    applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &Engine::OnActivated);

    CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &Engine::OnSuspending);

    CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &Engine::OnResuming);


#ifdef SIMPLE_SPRITES
	m_renderer = ref new SimpleSprites();
#endif // SIMPLE_SPRITES

}

void Engine::SetWindow(
    _In_ CoreWindow^ window
    )
{
    window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

    window->SizeChanged +=
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &Engine::OnWindowSizeChanged);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &Engine::OnVisibilityChanged);

    window->Closed +=
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &Engine::OnWindowClosed);

    DisplayInformation::GetForCurrentView()->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &Engine::OnDpiChanged);

    // Disable all pointer visual feedback for better performance when touching.
    auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
    pointerVisualizationSettings->IsContactFeedbackEnabled = false;
    pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;

//#ifdef SIMPLE_SPRITES
//	m_renderer->Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
//#elif defined CONTROLLER_RENDERER
//	m_controllerRenderer->Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
//#else
    DirectXBase::Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
//#endif // SIMPLE_SPRITES
}

void Engine::Load(
    _In_ Platform::String^ entryPoint
    )
{
}



void Engine::Uninitialize()
{
}

void Engine::OnWindowSizeChanged(
    _In_ CoreWindow^ sender,
    _In_ WindowSizeChangedEventArgs^ args
    )
{
    UpdateForWindowSizeChange();

#ifdef SHOW_OVERLAY
    m_sampleOverlay->UpdateForWindowSizeChange();
	m_debugOverlay->UpdateForWindowSizeChange();
#endif // SHOW_OVERLAY
}

void Engine::OnVisibilityChanged(
    _In_ CoreWindow^ sender,
    _In_ VisibilityChangedEventArgs^ args
    )
{
    m_windowVisible = args->Visible;

	m_screenBuilder =
		new ScreenBuilder(
		m_window->Bounds.Width,
		m_window->Bounds.Height);

	// Use Builders
	m_screenBuilder->BuildScreen(&m_treeData);

	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height,
		8,
		7,
		&x,
		&y);

	// Putting instantiona here since need to wait for the
	//	window to be created to get the bounds.
	m_pPlayer = new Player();

	m_pPlayer->SetHorizontalOffset(x);
	m_pPlayer->SetVerticalOffset(y);
}

void Engine::OnWindowClosed(
    _In_ CoreWindow^ sender,
    _In_ CoreWindowEventArgs^ args
    )
{
    m_windowClosed = true;
}

void Engine::OnDpiChanged(_In_ DisplayInformation^ sender, _In_ Platform::Object^ args)
{
    SetDpi(sender->LogicalDpi);
}

void Engine::OnActivated(
    _In_ CoreApplicationView^ applicationView,
    _In_ IActivatedEventArgs^ args
    )
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void Engine::OnSuspending(
    _In_ Platform::Object^ sender,
    _In_ SuspendingEventArgs^ args
    )
{
    // Hint to the driver that the app is entering an idle state and that its memory
    // can be temporarily used for other apps.
    Trim();
}

void Engine::OnResuming(
    _In_ Platform::Object^ sender,
    _In_ Platform::Object^ args
    )
{
}

void Engine::DrawPlayer()
{
	float x = 0.0f;
	float y = 0.0f;

	m_orchiData.pos.x = m_pPlayer->GetHorizontalOffset();
	m_orchiData.pos.y = m_pPlayer->GetVerticalOffset();

	float tempRot = 0.0f;
	float tempMag = 0.0f;
	m_orchiData.vel.x = tempMag * cosf(tempRot);
	m_orchiData.vel.y = tempMag * sinf(tempRot);
	m_orchiData.rot = 0.0f;
	m_orchiData.scale = 1.0f;
	m_orchiData.rotVel = 0.0f;
}

void Engine::DrawGrid()
{
	float windowWidth = m_window->Bounds.Width;
	float windowHeight = m_window->Bounds.Height;

	float gridWidth = windowWidth -
		(windowWidth * LEFT_MARGIN_RATIO) -
		(windowWidth * RIGHT_MARGIN_RATIO);

	// TODO: Use arrays instead of separate variables.
	float rowHeight = (windowHeight - 2.0f * MARGIN) / NUM_GRID_ROWS;
	float columnWidth = (gridWidth - 2.0f * MARGIN) / NUM_GRID_COLUMNS;

	// Draw the horizontal lines.
	for (int row = 0; row <= NUM_GRID_ROWS; row++)
	{
		D2D1_POINT_2F src
		{ 
			(windowWidth * LEFT_MARGIN_RATIO) + MARGIN, 
			MARGIN + (rowHeight * (float) row)
		};

		D2D1_POINT_2F dst
		{ 
			windowWidth - (windowWidth * RIGHT_MARGIN_RATIO) - MARGIN, 
			MARGIN + (rowHeight * (float) row)
		};

		m_d2dContext->DrawLine(src, dst, m_blackBrush.Get());
	}

	// Draw the vertical lines.
	for (int column = 0; column <= NUM_GRID_COLUMNS; column++)
	{
		D2D1_POINT_2F src
		{
			(windowWidth * LEFT_MARGIN_RATIO) + MARGIN + (columnWidth * (float) column),
			MARGIN
		};

		D2D1_POINT_2F dst
		{
			(windowWidth * LEFT_MARGIN_RATIO) + MARGIN + (columnWidth * (float) column),
			windowHeight - MARGIN,
		};

		m_d2dContext->DrawLine(src, dst, m_blackBrush.Get());
	}
}

void Engine::DrawTree()
{
	/*
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height,
		0, 0, &x, &y);

	Tree tree{ 0, 0, x, y, m_greenBrush };
	tree.Draw(m_d2dContext);
*/
}

void Engine::DrawRock()
{
	/*
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height, 1, 1, &x, &y);

	Rock rock{ 0, 0, x, y, m_grayBrush };
	rock.Draw(m_d2dContext);
*/
}

void Engine::DrawDoor()
{
	/*
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height, 2, 2, &x, &y);

	Door door{ 0, 0, x, y, m_blackBrush };
	door.Draw(m_d2dContext);
*/
}

void Engine::DrawGround()
{
	/*
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height, 3, 3, &x, &y);

	Ground ground{ 0, 0, x, y, m_beigeBrush };
	ground.Draw(m_d2dContext);
*/
}

void Engine::DrawWater()
{
	/*
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height, 8, 8, &x, &y);

	Water water{ 0, 0, x, y, m_blueBrush };
	water.Draw(m_d2dContext);
*/
}


IFrameworkView^ DirectXAppSource::CreateView()
{
    return ref new Engine();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto directXAppSource = ref new DirectXAppSource();
    CoreApplication::Run(directXAppSource);
    return 0;
}

void Engine::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();
}

void Engine::DrawLeftMargin()
{
	LeftMargin leftMargin;

	D2D1_RECT_F rect
	{
		0.0f,
		0.0f,
		m_window->Bounds.Width * LEFT_MARGIN_RATIO,
		m_windowBounds.Height
	};

	leftMargin.Draw(
		m_d2dContext,
		m_blackBrush.Get(),
		rect);
}

void Engine::DrawRightMargin()
{
	RightMargin rightMargin;

	D2D1_RECT_F rect
	{
		m_windowBounds.Width - (m_windowBounds.Width * RIGHT_MARGIN_RATIO),
		0.0f,
		m_windowBounds.Width,
		m_windowBounds.Height
	};

	rightMargin.Draw(
		m_d2dContext,
		m_blackBrush.Get(),
		rect);
}

void Engine::RenderControllerInput()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();
	D2D1_RECT_F pos = D2D1::RectF(INFORMATION_START_X, INFORMATION_START_Y, size.width, size.height);

//	m_d2dContext->BeginDraw();

	if (!m_isControllerConnected)
	{
		//
		// Display message instructing user to connect a controller.
		//
		DrawText(MSG_NEED_CONTROLLER, pos);
	}
	else
	{
		//
		// Display State information
		//

		// Labels
		DrawHeader(STATE_HEADER, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_PACKET_NUMBER, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_LEFT_TRIGGER, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_RIGHT_TRIGGER, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_LEFT_THUMB_X, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_LEFT_THUMB_Y, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_RIGHT_THUMB_X, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_RIGHT_THUMB_Y, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_STATE_BUTTONS, pos);

		// Values
		pos.top += LINE_HEIGHT;
//		pos.top = INFORMATION_START_Y + LINE_HEIGHT;
//		pos.left = INPUT_DATA_START;
		DrawText(static_cast<uint32>(m_xinputState.dwPacketNumber), pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.bLeftTrigger, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.bRightTrigger, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbLX, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbLY, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbRX, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbRY, pos);
		pos.top += LINE_HEIGHT;
		DrawButtonText(m_xinputState.Gamepad.wButtons, pos);

		//
		// Display Capabilties
		//

		// Labels
		pos.top += LINE_HEIGHT;
//		pos.left = CAPS_LABEL_START;
		DrawHeader(CAPS_HEADER, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_CAPS_TYPE, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_CAPS_SUBTYPE, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_CAPS_FLAGS, pos);

		// Values
		pos.top += LINE_HEIGHT;
//		pos.left = CAPS_DATA_START;
		DrawText(m_xinputCaps.Type, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputCaps.SubType, pos);
		pos.top += LINE_HEIGHT;
		if (m_xinputCaps.Flags & XINPUT_CAPS_WIRELESS)
		{
			DrawText(VALUE_CAPS_WIRELESS, pos);
		}
		else
		{
			DrawText(VALUE_CAPS_WIRED, pos);
		}
		if (m_xinputCaps.Flags & XINPUT_CAPS_VOICE_SUPPORTED)
		{
			pos.top += LINE_HEIGHT;
			DrawText(VALUE_CAPS_VOICE_SUPPORT, pos);
		}
	}

	// We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	/*
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}
	*/
}





void Engine::DrawHeader(const char16* text, const D2D1_RECT_F& loc)
{
	m_d2dContext->DrawText(
		text,
		static_cast<UINT32>(::wcslen(text)),
		m_headerTextFormat.Get(),
		loc,
		m_textBrush.Get()
		);
}



void Engine::DrawText(const char16* text, const D2D1_RECT_F& loc)
{
	m_d2dContext->DrawText(
		text,
		static_cast<UINT32>(::wcslen(text)),
		m_dataTextFormat.Get(),
		loc,
		m_textBrush.Get()
		);
}



void Engine::DrawText(uint32 value, const D2D1_RECT_F& loc)
{
	char16 text[16];
	::_snwprintf_s(text, sizeof(text) / sizeof(char16), L"0x%08X", value);
	DrawText(text, loc);
}



void Engine::DrawText(int16 value, const D2D1_RECT_F& loc)
{
	char16 text[16];
	::_snwprintf_s(text, sizeof(text) / sizeof(char16), L"%05d", value);
	DrawText(text, loc);
}



void Engine::DrawText(uint8 value, const D2D1_RECT_F& loc)
{
	char16 text[8];
	::_snwprintf_s(text, sizeof(text) / sizeof(char16), L"0x%02X", value);
	DrawText(text, loc);
}



void Engine::DrawButtonText(uint16 buttons, const D2D1_RECT_F& loc)
{
	char16 text[64];
	size_t where = 0;
	if (buttons & XINPUT_GAMEPAD_A)
	{
		text[where++] = L'A';
	}
	if (buttons & XINPUT_GAMEPAD_B)
	{
		text[where++] = L'B';
	}
	if (buttons & XINPUT_GAMEPAD_X)
	{
		text[where++] = L'X';
	}
	if (buttons & XINPUT_GAMEPAD_Y)
	{
		text[where++] = L'Y';
	}
	if (where != 0)
	{
		text[where++] = L' ';
	}

	size_t groupStart = where;

	if (where != groupStart)
	{
		text[where++] = L' ';
	}

	if (buttons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		text[where++] = L'L';
		text[where++] = L'T';
		text[where++] = L' ';
	}
	if (buttons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		text[where++] = L'R';
		text[where++] = L'T';
		text[where++] = L' ';
	}
	if (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		text[where++] = L'L';
		text[where++] = L'S';
		text[where++] = L' ';
	}
	if (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		text[where++] = L'R';
		text[where++] = L'S';
		text[where++] = L' ';
	}
	if (buttons & XINPUT_GAMEPAD_START)
	{
		text[where++] = L'S';
		text[where++] = L't';
		text[where++] = L'a';
		text[where++] = L'r';
		text[where++] = L't';
		text[where++] = L' ';
	}
	if (buttons & XINPUT_GAMEPAD_BACK)
	{
		text[where++] = L'B';
		text[where++] = L'a';
		text[where++] = L'c';
		text[where++] = L'k';
	}
	text[where] = L'\0';

	DrawText(text, loc);
}

// Need to isolate this functionality so that
//	the return can return to the Run() loop
//	rather than exiting the program.
void Engine::FetchControllerInput()
{
	if (!m_isControllerConnected)
	{
		//
		// Enumerating for XInput devices takes 'time' on the order of milliseconds.
		// Any time a device is not currently known as connected (not yet called XInput, or calling
		// an XInput function after a failure) ennumeration happens.
		// An app should avoid repeatedly calling XInput functions if there are no known devices connected
		// as this can slow down application performance.
		// This sample takes the simple approach of not calling XInput functions after failure
		// until a specified timeout has passed.
		//
		uint64 currentTime = ::GetTickCount64();
		if (currentTime - m_lastEnumTime < XINPUT_ENUM_TIMEOUT_MS)
		{
			return;
		}
		m_lastEnumTime = currentTime;

		// Check for controller connection by trying to get the capabilties
		uint32 capsResult = XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &m_xinputCaps);
		if (capsResult != ERROR_SUCCESS)
		{
			return;
		}

		// Device is connected
		m_isControllerConnected = true;
	}

	uint32 stateResult = XInputGetState(0, &m_xinputState);
	if (stateResult != ERROR_SUCCESS)
	{
		// Device is no longer connected
		m_isControllerConnected = false;
		m_lastEnumTime = ::GetTickCount64();
	}
}

int Engine::FetchKeyboardInput()
{
	return 1;
}

// TODO: Pass arrays rather than separate argument lists.
int Engine::CheckForCollisions(int * column, int * row)
{
	float2 size = m_spriteBatch->GetSpriteSize(m_orchi.Get());
	
	float left = m_pPlayer->GetHorizontalOffset() - size.x / 2.0f;
	float right = m_pPlayer->GetHorizontalOffset() + size.x / 2.0f;
	float top = m_pPlayer->GetVerticalOffset() - size.y / 2.0f;
	float bottom = m_pPlayer->GetVerticalOffset() + size.y / 2.0f;

	// Now I know the size (remember to scale accordingly, if needed using m_orchiData).

	// Look for collisions with all trees
	float2 treeSize = m_spriteBatch->GetSpriteSize(m_tree.Get());

	for (auto tree = m_treeData.begin(); tree != m_treeData.end(); tree++)
	{
		float treeLeft = tree->pos.x - treeSize.x / 2.0f;
		float treeRight = tree->pos.x + size.x / 2.0f;
		float treeTop = tree->pos.y - size.y / 2.0f;
		float treeBottom = tree->pos.y + size.y / 2.0f;

		// Does the top, left vertex overlap the tree's bounding box?
		if (left >= treeLeft &&
			left <= treeRight &&
			top >= treeTop &&
			top <= treeBottom)
		{
			*column = tree->column;
			*row = tree->row;
			return 1;
		}

		// Does the top, right vertex overlap the tree's bounding box?
		if (right >= treeLeft &&
			right <= treeRight &&
			top >= treeTop &&
			top <= treeBottom)
		{
			*column = tree->column;
			*row = tree->row;
			return 1;
		}

		// Does the bottom, right vertex overlap the tree's bounding box?
		if (right >= treeLeft &&
			right <= treeRight &&
			bottom >= treeTop &&
			bottom <= treeBottom)
		{
			*column = tree->column;
			*row = tree->row;
			return 1;
		}

		// Does the bottom, left vertex overlap the tree's bounding box?
		if (left >= treeLeft &&
			left <= treeRight &&
			bottom >= treeTop &&
			bottom <= treeBottom)
		{
			*column = tree->column;
			*row = tree->row;
			return 1;
		}
	}
	return 0;
}

// TODO: Could use function pointers.
void Engine::MovePlayer(uint16 buttons)
{
	if (buttons & XINPUT_GAMEPAD_DPAD_UP)
	{
		m_pPlayer->MoveNorth(m_window->Bounds.Height);
	}

	if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		m_pPlayer->MoveSouth(m_window->Bounds.Height);
	}

	if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		m_pPlayer->MoveWest(m_window->Bounds.Width);
	}

	if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		m_pPlayer->MoveEast(m_window->Bounds.Width);
	}
}

/*
	Highlight the sprite that is being collided with.
*/
void Engine::HighlightSprite(int column, int row)
{
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height, 
		row, 
		column,
		&x, 
		&y);

	D2D1_RECT_F rect
	{
		x - 50.f,
		y - 50.f,
		x + 50.f,
		y + 50.f
	};

	m_d2dContext->FillRectangle(
		rect,
		m_redBrush.Get());
}

void Engine::DisplaySpriteCollisionMessage(int column, int row)
{
	D2D1_RECT_F pos = D2D1::RectF(0.0f, 0.0f, 200.0f, 200.0f);
	DrawText(SPRITE_COLLISION_MSG, pos);
	pos.top += LINE_HEIGHT;
	DrawText(SPRITE_COLLISION_MSG_COLUMN, pos);
	pos.top += LINE_HEIGHT;
	DrawText(static_cast<uint32>(column), pos);
	pos.top += LINE_HEIGHT;

	DrawText(SPRITE_COLLISION_MSG_ROW, pos);
	pos.top += LINE_HEIGHT;
	DrawText(static_cast<uint32>(row), pos);
	pos.top += LINE_HEIGHT;
}


void Engine::Run()
{
	BasicTimer ^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();

			FetchControllerInput();
			MovePlayer(m_xinputState.Gamepad.wButtons);

			// if the gamepad is not connected, check the keyboard.
			if (!m_isControllerConnected)
			{
			}

			Render();
			Present();
		}
		else
		{
			CoreWindow::GetForCurrentThread()->
				Dispatcher->ProcessEvents(
				CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}