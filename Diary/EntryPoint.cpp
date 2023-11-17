#include <iostream>
#include <Windows.h>
#include <functional>

#include "Console.h"
#include "UI/MainMenu.h"
#include "FileManager.h"

int main()
{
	ConsoleStartupInfo* info = FileManager::LoadSettings();
	if (info == nullptr)
	{
		info = new ConsoleStartupInfo();
		info->Title = "Your BEST Diary 2.0";
		info->Fullscreen = true;
		info->Zoom = 22;
	}

	info->Title = u8"Můj milý deníček 2.0";
	if (info->Title == nullptr)
		info->Title = "My BEST Diary 2.0";

	info->StartupItem = new MainMenu();
	Console::Run(info);
}


