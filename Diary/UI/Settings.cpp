#include "Settings.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "../Console.h"
#include "MainMenu.h"

#include "../FileManager.h"

void Settings::MoveTo(int index, bool selected)
{
	if (m_XOffset <= 0)
		throw std::exception("The console window is too small");

	int computedIndex = index - (Console::BufferSize.Y / 2);
	// Upper/Bottom limit
	if (computedIndex == -4 || computedIndex == 6)
		return;

	// The Records whitespace above
	else if (computedIndex == -1) {
		index += 2;
	}
	else if (computedIndex == 0) {
		index -= 2;
	}
	// The Submit/Back whitespace above
	else if (computedIndex == 3)
	{
		if (m_CurrentItemIndex > index)
			index--;
		else
			index++;
	}
	// Submit and Back handling
	else if (selected) {
		// Submit Settings
		if (computedIndex == 4)
		{
			Console::SetZoom(m_Settings->Zoom);
			Console::Settings->Format = m_Settings->Format;
			Console::Settings->Sorting = m_Settings->Sorting;
			m_IsSettingValue = false;
			FileManager::SaveSettings();

			Console::SetFullscreen(m_Settings->Fullscreen, true);
			return;
		}
		// Back to Main Menu
		else if (computedIndex == 5)
		{
			m_IsSettingValue = false;
			delete m_Settings;
			Console::ChangeItem(new MainMenu(), true);
			return;
		}
	}

	if (index != m_CurrentItemIndex)
	{
		Console::SetCursorPosition(4 + m_XOffset, m_CurrentItemIndex);
		std::cout << "  ";
	}

	// TODO: Implement the upper/bottom limit to the arrow and accessible only the options that can be configured

	Console::SetCursorPosition(4 + m_XOffset, index);
	if (selected) {
		std::cout << "\x1B[31m" << ARROW_CHARACTERS << "\x1B[0m";
	}
	else
		std::cout << ARROW_CHARACTERS;

	m_CurrentItemIndex = index;
}

void Settings::Render()
{
	memcpy(m_Settings, Console::Settings, sizeof(ConsoleStartupInfo));

	Console::SetCursorPosition(0, (Console::BufferSize.Y / 2) - 10);
	m_CurrentItemIndex = (Console::BufferSize.Y / 2) - 3;
	m_XOffset = Console::BufferSize.X / 2 - 36;
	if (m_XOffset <= 0)
		return;

	std::string spaces = std::string(m_XOffset, ' ');

	std::cout << spaces + "Main Menu -> Settings\n";
	std::cout << spaces + "-------------------------------------------------------------------------\n";
	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "|\x1B[1m";
	std::cout << std::string(34 - (strlen(Console::Settings->Title) / 2), ' ') << Console::Settings->Title << "\x1B[0m";
	Console::SetCursorPosition(m_XOffset + 72, (Console::BufferSize.Y / 2) - 7);
	std::cout << "|\n";

	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "|      \x1B[4mGeneral\x1B[0m                                                          |\n";
	std::cout << spaces + "|   -> Fullscreen: " << (m_Settings->Fullscreen ? "True " : "False") << "                                                |\n";
	std::cout << spaces + "|      Zoom: " << std::setprecision(2) << (float)m_Settings->Zoom / (float)10 << "x";
	Console::SetCursorPosition(m_XOffset + 72, Console::BufferSize.Y / 2 - 2);
	std::cout << "|\n";

	// TODO: Add title editing

	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "|      \x1B[4mRecords\x1B[0m                                                          |\n";
	std::cout << spaces + "|      Records Format: ." << (m_Settings->Format == RecordFormat::JSON ? "json" : m_Settings->Format == RecordFormat::CSV ? "csv" : "txt");
	Console::SetCursorPosition(m_XOffset + 72, Console::BufferSize.Y / 2 + 1);
	std::cout << "|\n";

	std::cout << spaces + "|      Records Sorting: " << (m_Settings->Sorting == RecordSorting::None ? "None" : m_Settings->Sorting == RecordSorting::Date ? "Date" : "Title");
	Console::SetCursorPosition(m_XOffset + 72, Console::BufferSize.Y / 2 + 2);
	std::cout << "|\n";

	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "|      Submit                                                           |\n";
	std::cout << spaces + "|      Back                                                             |\n";
	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "|      Version 2.0                                                      |\n";
	std::cout << spaces + "|                                                                       |\n";
	std::cout << spaces + "-------------------------------------------------------------------------\n";
}

void Settings::TakeAction(bool up)
{
	if (m_XOffset <= 0)
		throw std::exception("The console window is too small");

	switch (m_CurrentItemIndex - (Console::BufferSize.Y / 2)) {
		// Fullscreen
	case -3:
		m_Settings->Fullscreen = !m_Settings->Fullscreen;
		if (m_Settings->Fullscreen)
			std::cout << " Fullscreen: True ";
		else
			std::cout << " Fullscreen: False";

		MoveTo(m_CurrentItemIndex, true);
		break;
		// Zoom
	case -2:
		if (up)
			m_Settings->Zoom += 1;
		else
			m_Settings->Zoom -= 1;

		std::cout << " Zoom: " << std::setprecision(2) << (float)m_Settings->Zoom / (float)10 << "x       ";

		MoveTo(m_CurrentItemIndex, true);
		break;

		// Records Format
	case 1:
	{
		int value = (int)m_Settings->Format;
		if (up)
			value++;
		else
			value--;

		if (value == RecordFormat_MAX_VALUE)
			value = JSON;
		else if (value == -1)
			value = RecordFormat_MAX_VALUE - 1;

		m_Settings->Format = (RecordFormat)value;
		switch (m_Settings->Format)
		{
		case RecordFormat::JSON:
			std::cout << " Records Format: .json";
			break;

		case RecordFormat::CSV:
			std::cout << " Records Format: .csv ";
			break;

		case RecordFormat::TXT:
			std::cout << " Records Format: .txt ";
			break;
		}

		MoveTo(m_CurrentItemIndex, true);
		break;
	}

	// Record Sorting
	case 2:
	{
		int value = (int)m_Settings->Sorting;
		if (up)
			value++;
		else
			value--;

		if (value == RecordSorting_MAX_VALUE)
			value = JSON;
		else if (value == -1)
			value = RecordSorting_MAX_VALUE - 1;

		m_Settings->Sorting = (RecordSorting)value;
		switch (m_Settings->Sorting)
		{
		case RecordSorting::None:
			std::cout << " Records Sorting: None ";
			break;

		case RecordSorting::Date:
			std::cout << " Records Sorting: Date ";
			break;

		case RecordSorting::Title:
			std::cout << " Records Sorting: Title";
			break;
		}

		MoveTo(m_CurrentItemIndex, true);
		break;
	}
	break;
	}
}

void Settings::OnKeyInput(KEY_EVENT_RECORD* eventItem)
{
	if (!eventItem->bKeyDown)
		return;

	switch (eventItem->wVirtualKeyCode) {
	case VK_DOWN:
		if (!m_IsSettingValue)
			MoveTo(m_CurrentItemIndex + 1);
		else
			TakeAction(false);
		break;

	case VK_UP:
		if (!m_IsSettingValue)
			MoveTo(m_CurrentItemIndex - 1);
		else
			TakeAction(true);
		break;

	case VK_RETURN:
	case VK_RIGHT:
		m_IsSettingValue = true;
		MoveTo(m_CurrentItemIndex, true);
		break;

	case VK_ESCAPE:
	case VK_BACK:
	case VK_LEFT:
		if (!m_IsSettingValue && (eventItem->wVirtualKeyCode == VK_BACK || eventItem->wVirtualKeyCode == VK_ESCAPE))
		{
			Console::ChangeItem(new MainMenu(), true);
			return;
		}

		m_IsSettingValue = false;
		MoveTo(m_CurrentItemIndex, false);
		break;
	}
}

void Settings::OnMouseInput(MOUSE_EVENT_RECORD* eventItem)
{

}

