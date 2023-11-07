#include "FileManager.h"

#include <ShlObj.h>
#include <string>
#include <fstream>
#include <sys/stat.h>

NodeList<Record> FileManager::Records = NodeList<Record>();

void FileManager::SaveSettings()
{
	std::wstring filePath = std::wstring(GetAppDataFolder());
	filePath += L"\\DetokenStudios";
	CreateDirectoryW(filePath.c_str(), NULL);
	filePath += L"\\Diary\\";
	CreateDirectoryW(filePath.c_str(), NULL);

	filePath += L"config.cfg";
	DeleteFileW(filePath.c_str());

	std::ofstream fileOutput;
	fileOutput.open(filePath.c_str(), std::fstream::out);
	fileOutput << "Title=" << Console::Settings->Title << '\n';
	fileOutput << "Fullscreen=" << (Console::Settings->Fullscreen ? "True" : "False") << '\n';
	fileOutput << "Zoom=" << Console::Settings->Zoom << '\n';
	fileOutput << "Format=" << (Console::Settings->Format == RecordFormat::JSON ? ".json" : Console::Settings->Format == RecordFormat::CSV ? ".csv" : ".txt") << '\n';
	fileOutput << "Sorting=" << (Console::Settings->Sorting == RecordSorting::None ? "None" : Console::Settings->Sorting == RecordSorting::Date ? "Date" : "Title") << '\n';

	fileOutput.flush();
	fileOutput.close();
}
ConsoleStartupInfo* FileManager::LoadSettings()
{
	std::wstring filePath = std::wstring(GetAppDataFolder());
	filePath += L"\\DetokenStudios\\Diary\\config.cfg";

	struct stat sb;
	if (_wstat(filePath.c_str(), (struct _stat64i32*)&sb) != 0)
		return nullptr;

	ConsoleStartupInfo* info = new ConsoleStartupInfo();
	std::fstream file;
	file.open(filePath.c_str(), std::fstream::in);

	std::string line;
	while (std::getline(file, line))
	{
		size_t middle = line.find('=', 0) + 1;
		std::string name = line.substr(0, middle - 1);
		line = line.substr(middle);
		if (name == "Fullscreen")
		{
			if (line == "True")
				info->Fullscreen = true;
			else
				info->Fullscreen = false;
		}
		else if (name == "Title")
		{
			const char* title = line.c_str();
			char* copyTitle = (char*)malloc(strlen(title) + 1);
			strcpy_s(copyTitle, strlen(title) + 1, title);
			info->Title = copyTitle;
		}
		else if (name == "Zoom") {
			info->Zoom = std::stoi(line);
		}
		else if (name == "Format") {
			if (line == ".json")
				info->Format = RecordFormat::JSON;
			else if (line == ".csv")
				info->Format = RecordFormat::CSV;
			else if (line == ".txt")
				info->Format = RecordFormat::TXT;
			else
				throw std::exception("Unknown Format for records");
		}
		else if (name == "Sorting") {
			if (line == "Date")
				info->Sorting = RecordSorting::Date;
			else if (line == "Title")
				info->Sorting = RecordSorting::Title;
			else
				info->Sorting = RecordSorting::None;
		}
	}

	file.close();
	return info;
}

void FileManager::SaveRecords()
{

}

void FileManager::LoadRecords()
{
	switch (Console::Settings->Format)
	{
	case RecordFormat::JSON:

		break;

	case RecordFormat::CSV:

		break;

	case RecordFormat::TXT:

		break;
	}
}

const wchar_t* FileManager::GetAppDataFolder()
{
	TCHAR* appDataFolder = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);;
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataFolder)))
		return nullptr;

	return appDataFolder;
}
