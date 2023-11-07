#pragma once
#include "Renderable.h"

#define ARROW_CHARACTERS "->"

enum RecordFormat {
	JSON,
	CSV,
	TXT,
	RecordFormat_MAX_VALUE
};
enum RecordSorting {
	None,
	Date,
	Title,
	RecordSorting_MAX_VALUE
};

struct ConsoleStartupInfo {
public:
	const char* Title;
	bool Fullscreen;
	int Zoom;
	RecordFormat Format = RecordFormat::JSON;
	RecordSorting Sorting = RecordSorting::None;
	Renderable* StartupItem;
};

class Console
{
public:
	static void Run(ConsoleStartupInfo* startupInfo);
	static void Clear();

	static void SetTitle(const char* title);
	static void SetFullscreen(bool fullscreen, bool reRender = false);
	static void SetZoom(int zoom);

	static void SetCursorPosition(int x, int y);
	static void SetCursorPosition(COORD* coord);
	static COORD GetCursorPosition();

	static void SetCursorVisibility(bool visible);

	static void ChangeItem(Renderable* item, bool clearConsole = false, bool freePrevious = true);

	static void Exit(int code);

	static COORD BufferSize;
	static ConsoleStartupInfo* Settings;
private:
	static void Render();
	static void QueryInput();

	static HANDLE m_STD_IN;
	static HANDLE m_STD_OUT;
	static HWND m_WindowHandle;
	static Renderable* m_CurrentItem;

	static int m_CurrentZoom;
	static bool m_IsFullscreen;
	static bool m_ShouldRun;
};
