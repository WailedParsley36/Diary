#include "Console.h"

#include <iostream>
#include "FileManager.h"

COORD Console::BufferSize;
ConsoleStartupInfo* Console::Settings;

bool Console::m_ShouldRun = true;
bool Console::m_IsFullscreen = false;
int Console::m_CurrentZoom = 10;
HANDLE Console::m_STD_IN;
HANDLE Console::m_STD_OUT;
HWND Console::m_WindowHandle;
Renderable* Console::m_CurrentItem;

void Console::Run(ConsoleStartupInfo* startupInfo)
{
	Settings = startupInfo;
	m_STD_IN = GetStdHandle(STD_INPUT_HANDLE);
	m_STD_OUT = GetStdHandle(STD_OUTPUT_HANDLE);
	m_WindowHandle = GetConsoleWindow();
	SetConsoleOutputCP(65001);

	//printf(u8"Testing unicode -- English -- Ελληνικά -- Español -- Русский. aäbcdefghijklmnoöpqrsßtuüvwxyz - aábcčdď\n");

	Console::m_CurrentItem = startupInfo->StartupItem;

	if (startupInfo->Title == nullptr)
		throw std::exception("The application needs to have a title");

	Console::SetTitle(startupInfo->Title);
	Console::SetCursorVisibility(false);
	Console::SetFullscreen(Console::Settings->Fullscreen);

	DWORD consoleMode;
	if (!GetConsoleMode(m_STD_IN, &consoleMode))
		return;

	consoleMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(m_STD_IN, consoleMode))
		return;

	while (m_ShouldRun) {
		Console::QueryInput();
	}
}
void Console::Render()
{
	Console::SetCursorPosition(0, 0);
	m_CurrentItem->Render();
}
void Console::ChangeItem(Renderable* item, bool clearConsole, bool freePrevious)
{
	if (freePrevious)
		delete Console::m_CurrentItem;

	Console::m_CurrentItem = item;
	if (clearConsole)
		Console::Clear();

	Console::Render();
}

void Console::Exit(int code)
{
	FileManager::SaveSettings();

	ShowWindow(m_WindowHandle, 0);
	FreeConsole();
	CloseWindow(m_WindowHandle);
	exit(code);
}

void Console::SetTitle(const char* title)
{
	SetConsoleTitleA(title);
}
void Console::SetFullscreen(bool fullscreen, bool reRender)
{
	// If my boolean is in the same state then no change is needed
	if (fullscreen == m_IsFullscreen)
		return;

	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

	//ShowWindow(m_WindowHandle, SW_SHOWMAXIMIZED);
	Settings->Fullscreen = fullscreen;
	m_IsFullscreen = fullscreen;

	if (reRender)
	{
		Console::Clear();
		Console::Render();
	}
}
void Console::SetZoom(int zoom)
{
	// Calculate the center of the window.
	RECT rect = RECT();
	GetWindowRect(GetParent(m_WindowHandle), &rect);

	POINT p = POINT();
	p.x = (rect.left + rect.right) / 2;
	p.y = (rect.top + rect.bottom) / 2;


	for (int i = abs(zoom - m_CurrentZoom); i > 0; i--)
	{
		SetCursorPos(p.x, p.y);
		keybd_event(VK_LCONTROL, 0x1d, 0, 0);
		mouse_event(MOUSEEVENTF_WHEEL, 10, 10, zoom > m_CurrentZoom ? WHEEL_DELTA : -WHEEL_DELTA, 0);
		keybd_event(VK_LCONTROL, 0x1d, KEYEVENTF_KEYUP, 0);
	}

	m_CurrentZoom = zoom;
	Console::Settings->Zoom = zoom;
}

void Console::QueryInput()
{
	INPUT_RECORD input;
	DWORD read;
	if (!ReadConsoleInputW(m_STD_IN, &input, 1, &read))
		return;

	if (input.EventType == KEY_EVENT)
		m_CurrentItem->OnKeyInput(&input.Event.KeyEvent);
	else if (input.EventType == MOUSE_EVENT)
		m_CurrentItem->OnMouseInput(&input.Event.MouseEvent);
	else if (input.EventType == WINDOW_BUFFER_SIZE_EVENT)
	{
		BufferSize = input.Event.WindowBufferSizeEvent.dwSize;

		Console::Clear();
		Console::Render();

		if (m_CurrentZoom != Console::Settings->Zoom)
		{
			Sleep(500);
			Console::SetZoom(Console::Settings->Zoom);
		}
	}
}
void Console::Clear()
{
	system("cls");
}

#pragma region Cursor
void Console::SetCursorPosition(int x, int y)
{
	COORD coord = COORD();
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(m_STD_OUT, coord);
}
void Console::SetCursorPosition(COORD* coord)
{
	SetConsoleCursorPosition(m_STD_OUT, *coord);
}
COORD Console::GetCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	GetConsoleScreenBufferInfo(m_STD_OUT, &cbsi);

	return cbsi.dwCursorPosition;
}

void Console::SetCursorVisibility(bool visible)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(m_STD_OUT, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(m_STD_OUT, &cursorInfo);
}
#pragma endregion
