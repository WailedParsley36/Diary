#pragma once
#include "Console.h"

#include "Data/NodeList.h"
#include "Data/Record.h"

class FileManager
{
public:
	static NodeList<Record> Records;

	static void SaveSettings();
	static ConsoleStartupInfo* LoadSettings();

	static void SaveRecords();
	static void LoadRecords();

private:
	static const wchar_t* GetAppDataFolder();
};

