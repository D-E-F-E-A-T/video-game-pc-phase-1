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
#include "BoundingBoxCornerCollisionStrategy.h"
#include "BoundingBoxMidpointCollisionStrategy.h"
#include "SpriteOverlapCollisionStrategy.h"
#include "ScreenUtils.h"
#include "LifePanel.h"
#include "GridSpace.h"
#include "BroadCollisionStrategy.h"

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
	const char16    APPLICATION_TITLE[] = L"XInput game controller sample";

	const float     CLEAR_COLOR[4] = { 0.071f, 0.040f, 0.561f, 1.0f };

	const char16    FONT_LOCAL[] = L"en-US";
	const char16    FONT_NAME[] = L"Segoe UI";
	const float     FONT_SIZE_HEADER = 18.0f;
	const float     FONT_SIZE_TEXT = 18.0f;

	const float     INFORMATION_START_X = 32.0f;
	const float     INFORMATION_START_Y = 150.0f;
	const float     LINE_HEIGHT = FONT_SIZE_HEADER * 1.5f;

	const float     INPUT_LABEL_START = INFORMATION_START_X;
	const float     INPUT_DATA_START = INFORMATION_START_X + 132.0f;
	const float     CAPS_LABEL_START = INFORMATION_START_X + 300.0f;
	const float     CAPS_DATA_START = INFORMATION_START_X + 400.0f;

	const char16    MSG_NEED_CONTROLLER[] = L"Please attach an Xbox 360 common controller device.";
	const char16	SPRITE_COLLISION_MSG[] = L"Sprite collision:";
	const char16	SPRITE_COLLISION_MSG_ROW[] = L"Row";
	const char16	SPRITE_COLLISION_MSG_COLUMN[] = L"Column";
	//	const uint64    XINPUT_ENUM_TIMEOUT_MS = 2000;  // 2 seconds

	const char16    STATE_HEADER[] = L"XInput State";
	const char16    LABEL_STATE_PACKET_NUMBER[] = L"Packet Number";
	const char16    LABEL_STATE_LEFT_TRIGGER[] = L"Left Trigger";
	const char16    LABEL_STATE_RIGHT_TRIGGER[] = L"Right Trigger";
	const char16    LABEL_STATE_LEFT_THUMB_X[] = L"Left Thumb X";
	const char16    LABEL_STATE_LEFT_THUMB_Y[] = L"Left Thumb Y";
	const char16    LABEL_STATE_RIGHT_THUMB_X[] = L"Right Thumb X";
	const char16    LABEL_STATE_RIGHT_THUMB_Y[] = L"Right Thumb Y";
	const char16    LABEL_STATE_BUTTONS[] = L"Buttons";

	const char16    CAPS_HEADER[] = L"XInput Capabilties";
	const char16    LABEL_CAPS_TYPE[] = L"Type";
	const char16    LABEL_CAPS_SUBTYPE[] = L"Subtype";
	const char16    LABEL_CAPS_FLAGS[] = L"Flags";
	const char16    VALUE_CAPS_WIRED[] = L"Wired";
	const char16    VALUE_CAPS_WIRELESS[] = L"Wireless";
	const char16    VALUE_CAPS_VOICE_SUPPORT[] = L"Voice Support";
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
	m_broadCollisionDetectionStrategy =
		//		new BoundingBoxCornerCollisionStrategy();
		//new SpriteOverlapCollisionStrategy();
		new BroadCollisionStrategy();

	m_pNarrowCollisionDetectionStrategy =
		new NarrowCollisionStrategy();
		

	m_pKeyboardController = new KeyboardControllerInput();

	m_pCollided = new list<BaseSpriteData *>;
	m_pTreeData = new std::vector<BaseSpriteData *>;
}

void Engine::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			64.0f,
			L"en-US",
			&m_textFormat)
		);

}

void Engine::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

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
			D2D1::ColorF(D2D1::ColorF::Yellow),
			&m_yellowBrush
			)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Green),
			&m_greenBrush
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
			D2D1::ColorF(D2D1::ColorF::SkyBlue),
			&m_blueBrush)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&m_redBrush)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Purple),
			&m_purpleBrush)
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
//		"test.dds",
		&m_orchi,
		nullptr);

	m_spriteBatch->AddTexture(m_orchi.Get());

	loader->LoadTexture(
		"heart.dds",
		&m_heart,
		nullptr);

	m_spriteBatch->AddTexture(m_heart.Get());

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

	// See DirectX marble maze sample
	window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs ^>(this, &Engine::OnKeyDown);

	DisplayInformation::GetForCurrentView()->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &Engine::OnDpiChanged);

	// Disable all pointer visual feedback for better performance when touching.
	auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
	pointerVisualizationSettings->IsContactFeedbackEnabled = false;
	pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;

	DirectXBase::Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);

	grid.SetWindowWidth(m_window->Bounds.Width);
	grid.SetWindowHeight(m_window->Bounds.Height);
	grid.SetNumColumns(NUM_GRID_COLUMNS);
	grid.SetNumRows(NUM_GRID_ROWS);

	this->m_pPlayer = new Player(&grid);
}

void Engine::Load(
	_In_ Platform::String^ entryPoint
	)
{
}



void Engine::Uninitialize()
{
}

void Engine::BuildScreen()
{
	delete m_screenBuilder;

	m_screenBuilder =
		new ScreenBuilder(
			m_window->Bounds.Width,
			m_window->Bounds.Height);

	// Use chain-of-responsibility?
	m_screenBuilder->BuildScreen(m_pTreeData);

	LifePanel lifePanel(
		m_window->Bounds.Width - m_window->Bounds.Width * RIGHT_MARGIN_RATIO,
		m_window->Bounds.Height * HEART_PANEL_HEIGHT_RATIO,
		m_window->Bounds.Width * RIGHT_MARGIN_RATIO,
		HEART_PANEL_HEIGHT);

	lifePanel.BuildPanel(&m_heartData);
}

void Engine::OnWindowSizeChanged(
	_In_ CoreWindow^ sender,
	_In_ WindowSizeChangedEventArgs^ args
	)
{
	UpdateForWindowSizeChange();

	grid.SetWindowWidth(m_window->Bounds.Width);
	grid.SetWindowHeight(m_window->Bounds.Height);

	BuildScreen();


}

void Engine::OnVisibilityChanged(
	_In_ CoreWindow^ sender,
	_In_ VisibilityChangedEventArgs^ args
	)
{
	m_windowVisible = args->Visible;
	BuildScreen();
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

	// Want player to move with same speed when
	//	moving vertically or horizontally.
	//	Thus, don't consider the side margins when
	//	multiplying by the player's location ratios.

	m_orchiData.pos.x = ((m_window->Bounds.Width -
		m_window->Bounds.Width * LEFT_MARGIN_RATIO -
		m_window->Bounds.Width * RIGHT_MARGIN_RATIO) *
		m_pPlayer->GetHorizontalRatio()) +
		(m_window->Bounds.Width * LEFT_MARGIN_RATIO);

	m_orchiData.pos.y = m_window->Bounds.Height * m_pPlayer->GetVerticalRatio();

	float tempRot = 0.0f;
	float tempMag = 0.0f;
	m_orchiData.vel.x = tempMag * cosf(tempRot);
	m_orchiData.vel.y = tempMag * sinf(tempRot);
	m_orchiData.rot = 0.0f;
	m_orchiData.scale = 1.0f;
	m_orchiData.rotVel = 0.0f;
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

	CreateLifeText();
	CreateButtonsText();

	CreateMapText();
	CreateInventoryText();
	CreatePackText();
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
		m_window->Bounds.Width - (m_window->Bounds.Width * RIGHT_MARGIN_RATIO),
		0.0f,
		m_window->Bounds.Width,
		m_window->Bounds.Height
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
		DrawText(static_cast<uint32>(m_xinputState.dwPacketNumber), pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.bLeftTrigger, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.bRightTrigger, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbLX, pos);	// Left thumb
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbLY, pos);	// Right thumb
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbRX, pos);
		pos.top += LINE_HEIGHT;
		DrawText(m_xinputState.Gamepad.sThumbRY, pos);
		pos.top += LINE_HEIGHT;
		DrawButtonText(m_xinputState.Gamepad.wButtons, pos);

		// Labels
		pos.top += LINE_HEIGHT;
		DrawHeader(CAPS_HEADER, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_CAPS_TYPE, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_CAPS_SUBTYPE, pos);
		pos.top += LINE_HEIGHT;
		DrawText(LABEL_CAPS_FLAGS, pos);

		// Values
		pos.top += LINE_HEIGHT;
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

// TODO: Could use function pointers.
void Engine::MovePlayer(uint16 buttons, short horizontal, short vertical)
{
	if (buttons & XINPUT_GAMEPAD_DPAD_UP)
	{
		m_pPlayer->MoveNorth(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		m_pPlayer->MoveSouth(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		m_pPlayer->MoveWest(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		m_pPlayer->MoveEast(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else
	{
		HandleLeftThumbStick(horizontal, vertical);
	}
}


void Engine::HighlightSprite(int * pLocation, ComPtr<ID2D1SolidColorBrush> brush)
{
	if (pLocation)
		HighlightSprite(
			pLocation[HORIZONTAL_AXIS], 
			pLocation[VERTICAL_AXIS], brush);
}

/*
Highlight the sprite that is being collided with.
*/
void Engine::HighlightSprite(int column, int row, ComPtr<ID2D1SolidColorBrush> brush)
{
	float x = 0.0f;
	float y = 0.0f;

	ScreenUtils::CalculateSquareCenter(
		m_window->Bounds.Width,
		m_window->Bounds.Height,
		column,
		row,
		&x,
		&y);

	float gameAreaWidth = 
		m_window->Bounds.Width - 
		(m_window->Bounds.Width * LEFT_MARGIN_RATIO) - 
		(m_window->Bounds.Width * RIGHT_MARGIN_RATIO);

	float gameAreaHeight = m_window->Bounds.Height;

	D2D1_RECT_F rect
	{
		x - (gameAreaWidth / (float)NUM_GRID_COLUMNS) / 2.0f,
		y - m_window->Bounds.Height / (float)NUM_GRID_ROWS / 2.0f,
		x + (gameAreaWidth / (float)NUM_GRID_COLUMNS) / 2.0f,
		y + m_window->Bounds.Height / (float)NUM_GRID_ROWS / 2.0f
	};


	m_d2dContext->FillRectangle(
		rect,
		brush.Get());
}

void Engine::DrawSpriteIntersection()
{
	D2D1_RECT_F rect
	{
		intersectRect[0],
		intersectRect[2],
		intersectRect[1],
		intersectRect[3]
	};


	if (m_nCollisionState == INTERSECTION)
	{
		m_d2dContext->FillRectangle(
			rect,
			m_yellowBrush.Get());
	}
	else if (m_nCollisionState == COLLISION)
	{
		m_d2dContext->FillRectangle(
			rect,
			m_greenBrush.Get());
	}
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

	DrawLifeText();
	DrawButtonsText();

	DrawMapText();
	DrawInventoryText();
	DrawPackText();





	// If the Player moves to the sides of the screen, scroll
	//	 and don't render the grid.

	grid.SetVisibility(true);

	grid.Draw(m_d2dContext, m_blackBrush);

	DrawPlayer();

	int column = 0;
	int row = 0;

	std::list<BaseSpriteData *>::const_iterator iterator;

#ifdef RENDER_DIAGNOSTICS

	for (iterator = m_pCollided->begin(); iterator != m_pCollided->end(); iterator++)
	{
		int column = (*iterator)->column;
		int row = (*iterator)->row;

		HighlightSprite(column, row, m_redBrush);
	}

	if (m_nCollisionState == INTERSECTION ||
		m_nCollisionState == COLLISION)
		DrawSpriteIntersection();
#endif // RENDER_DIAGNOSTICS

#ifdef DISPLAY_CONTROLLER_INPUT
	RenderControllerInput();
#endif // DISPLAY_CONTROLLER_INPUT

	// We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = m_d2dContext->EndDraw();

	m_d3dContext->OMSetRenderTargets(
		1,
		m_d3dRenderTargetView.GetAddressOf(),
		nullptr
		);

	DrawSprites();

	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}
}

void Engine::Run()
{
	BasicTimer ^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			// For Windows 10, need this so that the app window
			//	is responsive.
			m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			timer->Update();

			FetchControllerInput();

			float2 playerSize = m_spriteBatch->GetSpriteSize(m_orchi.Get());
			float2 spriteSize = m_spriteBatch->GetSpriteSize(m_tree.Get());

			float playerLocation[2];

			// These are within the range of screen pixel size.
			playerLocation[0] = (m_window->Bounds.Width - 
				(m_window->Bounds.Width * LEFT_MARGIN_RATIO) - 
				(m_window->Bounds.Width * RIGHT_MARGIN_RATIO)) * 
				m_pPlayer->GetHorizontalRatio() + 
				(m_window->Bounds.Width * LEFT_MARGIN_RATIO);

			playerLocation[1] = m_pPlayer->GetVerticalRatio() * m_window->Bounds.Height;

			m_broadCollisionDetectionStrategy->Detect(
				m_pCollided,
				playerSize,
				spriteSize,
				m_pPlayer,
				m_pTreeData,
				m_window->Bounds.Width,
				m_window->Bounds.Height,
				playerLocation);

			m_nCollisionState = m_pNarrowCollisionDetectionStrategy->Detect(
				m_d3dContext.Get(),
				m_d3dDevice.Get(),
				m_orchi.Get(),
				m_tree.Get(),
				m_pPlayer,
				m_pCollided,
				playerLocation,
				&grid,
				intersectRect);




			// if the gamepad is not connected, check the keyboard.
			if (m_isControllerConnected)
			{
				// This would actually, detect a collision one interation
				//	too late.  Consider detection earlier since
				//	this could lead to weird behavior, depending
				//	on how fast the sprites are moving.
				//	For example, if sprites are moving very quickly,
				//	collision would occur when the sprites 
				//	have deeply intersected each other.
				//  For slow moving sprites, this would not be 
				//	much of a problem.
				MovePlayer(
					m_xinputState.Gamepad.wButtons,
					m_xinputState.Gamepad.sThumbLX,
					m_xinputState.Gamepad.sThumbLY);
			}

			// OnKeyDown callback will check if the keyboard is used.

			Render();
			Present();

			m_pCollided->clear();
		}
		else
		{
			CoreWindow::GetForCurrentThread()->
				Dispatcher->ProcessEvents(
					CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void Engine::DrawSprites()
{
	m_spriteBatch->Begin();
	
	// @see: http://www.gamedev.net/topic/603359-c-dx11-how-to-get-texture-size/

	ID3D11Texture2D * pTextureInterface = NULL;

	std::vector<BaseSpriteData *>::const_iterator iterator;

	// This is a sprite run.
	for (iterator = m_pTreeData->begin(); iterator != m_pTreeData->end(); iterator++)
	{
		float fColumnWidth = grid.GetColumnWidth();
		float fRowHeight = grid.GetRowHeight();

		m_spriteBatch->Draw(
			m_tree.Get(),
			(*iterator)->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(fColumnWidth, fRowHeight),
			BasicSprites::SizeUnits::DIPs,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			(*iterator)->rot
			);
	}

	m_heart.Get()->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC heartDesc;
	pTextureInterface->GetDesc(&heartDesc);

	// This is a sprite run.
	for (auto heart = m_heartData.begin(); heart != m_heartData.end(); heart++)
	{
		m_spriteBatch->Draw(
			m_heart.Get(),
			heart->pos,
			BasicSprites::PositionUnits::DIPs,
			float2(
				((m_window->Bounds.Width - m_window->Bounds.Width * RIGHT_MARGIN_RATIO) / NUM_HEART_COLUMNS) / heartDesc.Width / 2.0f * 0.85f, 
				(HEART_PANEL_HEIGHT / NUM_HEART_ROWS) / heartDesc.Height) * 0.85f,
			BasicSprites::SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			heart->rot
			);
	}

	// This is a sprite run.
	m_spriteBatch->Draw(
		m_orchi.Get(),
		m_orchiData.pos,
		BasicSprites::PositionUnits::DIPs,
		float2(grid.GetColumnWidth(), grid.GetRowHeight()),	// This will stretch or shrink orchi.
		BasicSprites::SizeUnits::DIPs,
		float4(0.8f, 0.8f, 1.0f, 1.0f),
		m_orchiData.rot
		);

	m_spriteBatch->End();
}


void Engine::CreateLifeText()
{
	Platform::String ^ text = "Life";

	D2D1_SIZE_F size = m_d2dContext->GetSize();
	size.height = size.height / 3.0f;
	size.width = size.width * RIGHT_MARGIN_RATIO;

	ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			size.width,
			size.height,
			&textLayout
			)
		);

	textLayout.As(&m_textLayoutLife);

	m_textRange.startPosition = 0;
	m_textRange.length = text->Length();
	m_textLayoutLife->SetFontSize(SECTION_HEADER_FONT_SIZE, m_textRange);
	m_textLayoutLife->SetCharacterSpacing(0.5f, 0.5f, 0, m_textRange);
	m_textLayoutLife->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}


void Engine::CreateMapText()
{
	Platform::String ^ text = "Map";

	D2D1_SIZE_F size = m_d2dContext->GetSize();
	size.height = size.height / 3.0f;
	size.width = size.width * LEFT_MARGIN_RATIO;

	ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			size.width,
			size.height,
			&textLayout
			)
		);

	textLayout.As(&m_textLayoutMap);

	m_textRange.startPosition = 0;
	m_textRange.length = text->Length();
	m_textLayoutMap->SetFontSize(SECTION_HEADER_FONT_SIZE, m_textRange);
	m_textLayoutMap->SetCharacterSpacing(0.5f, 0.5f, 0, m_textRange);
	m_textLayoutMap->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void Engine::CreateButtonsText()
{
	Platform::String ^ text = "Buttons";

	D2D1_SIZE_F size = m_d2dContext->GetSize();
	size.height = size.height / 3.0f;
	size.width = size.width * RIGHT_MARGIN_RATIO;

	ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			size.width,
			size.height,
			&textLayout
			)
		);

	textLayout.As(&m_textLayoutButtons);

	m_textRange.startPosition = 0;
	m_textRange.length = text->Length();
	m_textLayoutButtons->SetFontSize(SECTION_HEADER_FONT_SIZE, m_textRange);
	m_textLayoutButtons->SetCharacterSpacing(0.5f, 0.5f, 0, m_textRange);
	m_textLayoutButtons->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void Engine::CreateInventoryText()
{
	Platform::String ^ text = "Inventory";

	D2D1_SIZE_F size = m_d2dContext->GetSize();
	size.height = size.height / 3.0f;
	size.width = size.width * LEFT_MARGIN_RATIO;

	ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			size.width,
			size.height,
			&textLayout
			)
		);

	textLayout.As(&m_textLayoutInventory);

	m_textRange.startPosition = 0;
	m_textRange.length = text->Length();
	m_textLayoutInventory->SetFontSize(SECTION_HEADER_FONT_SIZE, m_textRange);
	m_textLayoutInventory->SetCharacterSpacing(0.5f, 0.5f, 0, m_textRange);
	m_textLayoutInventory->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void Engine::CreatePackText()
{
	Platform::String ^ text = "Pack";

	D2D1_SIZE_F size = m_d2dContext->GetSize();
	size.height = size.height / 3.0f;
	size.width = size.width * RIGHT_MARGIN_RATIO;

	ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			size.width,
			size.height,
			&textLayout
			)
		);

	textLayout.As(&m_textLayoutPack);

	m_textRange.startPosition = 0;
	m_textRange.length = text->Length();
	m_textLayoutPack->SetFontSize(SECTION_HEADER_FONT_SIZE, m_textRange);
	m_textLayoutPack->SetCharacterSpacing(0.5f, 0.5f, 0, m_textRange);
	m_textLayoutPack->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void Engine::DrawLifeText()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();

	float fTop = size.height * 0.01f;
	float fLeft = size.width - (size.width * RIGHT_MARGIN_RATIO);

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(fLeft, fTop),
		m_textLayoutLife.Get(),
		m_whiteBrush.Get()
		);
}

void Engine::DrawMapText()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();

	float fTop = size.height * 0.01f;
	float fLeft = 0.0f;// size.width - (size.width * LEFT_MARGIN_RATIO);

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(fLeft, fTop),
		m_textLayoutMap.Get(),
		m_whiteBrush.Get()
		);
}

void Engine::DrawButtonsText()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();

	float fTop = size.height / 3.0f;
	float fLeft = size.width - (size.width * RIGHT_MARGIN_RATIO);

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(fLeft, fTop),
		m_textLayoutButtons.Get(),
		m_whiteBrush.Get()
		);
}

void Engine::DrawInventoryText()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();

	float fTop = size.height / 3.0f;
	float fLeft = 0.0f;

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(fLeft, fTop),
		m_textLayoutInventory.Get(),
		m_whiteBrush.Get()
		);
}

void Engine::DrawPackText()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();

	float fTop = size.height / 3.0f * 2.0f;
	float fLeft = 0.0f; // size.width - (size.width * LEFT_MARGIN_RATIO);

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(fLeft, fTop),
		m_textLayoutPack.Get(),
		m_whiteBrush.Get()
		);
}

// Since this in not in the main render/present loop, 
//	don't know when the narrow collision detection
//	algorithm will be activated when using
//	keyboard input.
void Engine::OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{
	if (args->VirtualKey == Windows::System::VirtualKey::P)       // Pause
	{
	}

	if (args->VirtualKey == Windows::System::VirtualKey::Left)
	{
		m_pPlayer->MoveWest(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else if (args->VirtualKey == Windows::System::VirtualKey::Down)
	{
		m_pPlayer->MoveSouth(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else if (args->VirtualKey == Windows::System::VirtualKey::Right)
	{
		m_pPlayer->MoveEast(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
	else if (args->VirtualKey == Windows::System::VirtualKey::Up)
	{
		m_pPlayer->MoveNorth(m_nCollisionState, PLAYER_MOVE_VELOCITY);
	}
}

void Engine::HandleLeftThumbStick(short horizontal, short vertical)
{
	float radius = sqrt((double)horizontal * (double)horizontal + (double)vertical * (double)vertical);
	float velocity = 0.f;

	if (radius < WALKING_THRESHOLD)
		return;
	if (radius >= WALKING_THRESHOLD && radius < RUNNING_THRESHOLD)
		velocity = PLAYER_MOVE_VELOCITY;
	else if (radius >= RUNNING_THRESHOLD)
		velocity = PLAYER_MOVE_VELOCITY * 2.0f;

	if (horizontal == 0)
	{
		if (vertical > 0)
		{
			m_pPlayer->MoveNorth(m_nCollisionState, velocity);
		}
		else if (vertical < 0)
		{
			m_pPlayer->MoveSouth(m_nCollisionState, velocity);
		}
	}
	else if (vertical == 0)
	{
		if (horizontal > 0)
		{
			m_pPlayer->MoveEast(m_nCollisionState, velocity);
		}
		else if (horizontal < 0)
		{
			m_pPlayer->MoveWest(m_nCollisionState, velocity);
		}
	}
	else
	{
		float param = (float)vertical / (float)horizontal;
		float theta = atan(param) * 180.0f / PI;

		if (horizontal > 0 && vertical > 0)
		{
			// Upper-right quadrant.
			if (theta <= 45.f)
				m_pPlayer->MoveEast(m_nCollisionState, velocity);
			else
				m_pPlayer->MoveNorth(m_nCollisionState, velocity);
		}
		else if (horizontal > 0 && vertical < 0)
		{
			// Lower-right quadrant.
			if (theta >= -45.f)
				m_pPlayer->MoveEast(m_nCollisionState, velocity);
			else
				m_pPlayer->MoveSouth(m_nCollisionState, velocity);
		}
		else if (horizontal < 0 && vertical > 0)
		{
			// Upper-left quadrant.
			if (theta >= -45.f)
				m_pPlayer->MoveWest(m_nCollisionState, velocity);
			else
				m_pPlayer->MoveNorth(m_nCollisionState, velocity);
		}
		else // (horizontal < 0 && vertical < 0)
		{
			// Lower-left quadrant.
			if (theta <= 45.f)
				m_pPlayer->MoveWest(m_nCollisionState, velocity);
			else
				m_pPlayer->MoveSouth(m_nCollisionState, velocity);
		}
	}
}