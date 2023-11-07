#include "Records.h"
#include "../FileManager.h"

#include "MainMenu.h"
#include "RecordDetail.h"

#include <time.h>
#include <ctime>
#include <iostream>

void Records::Render()
{
	m_XOffset = Console::BufferSize.X / 2 - 26;
	m_YOffset = Console::BufferSize.Y / 2 - 12;
	m_CurrentItemIndex = m_YOffset + 9;

	Console::SetCursorPosition(0, m_YOffset);
	std::string spaces = std::string(m_XOffset, ' ');

	int length = FileManager::Records.Length();

	std::cout << spaces + "Main Menu -> Records\n";
	std::cout << spaces + "---------------------------------------------------\n";
	std::cout << spaces + "|                                                 |\n";
	std::cout << spaces + "|\x1B[1m";
	std::cout << std::string(26 - (strlen(Console::Settings->Title) / 2), ' ') << Console::Settings->Title << "\x1B[0m";
	Console::SetCursorPosition(m_XOffset + 50, m_YOffset + 3);
	std::cout << "|\n";

	std::cout << spaces + "|                                                 |\n";
	std::cout << spaces + "|                                                 |\n";
	std::cout << spaces + "|      Record Count: " << length;
	Console::SetCursorPosition(m_XOffset + 50, m_YOffset + 6);
	std::cout << "|\n";

	std::cout << spaces + "|                                                 |\n";
	std::cout << spaces + "|      \x1B[4mOptions\x1B[0m                                    |\n";
	std::cout << spaces + "|   -> Create new                                 |\n";
	std::cout << spaces + "|      Find by title:                             |\n";
	std::cout << spaces + "|      Go to position:                            |\n";
	std::cout << spaces + "|      Go to date:                                |\n";
	std::cout << spaces + "|      Back                                       |\n";

	if (length > 0) {
		std::cout << spaces + "|                                                 |\n";
		std::cout << spaces + "|      \x1B[4mFirst Record\x1B[0m                               |\n";
		std::cout << spaces + "|      Title: \x1B[3m" << FileManager::Records.Head->Value->Title << "\x1B[0m";
		Console::SetCursorPosition(m_XOffset + 50, m_YOffset + 16);
		std::cout << "|\n";

		std::cout << spaces + "|      Date: \x1B[3m" << FileManager::Records.Head->Value->Time.tm_mday << '.' << FileManager::Records.Head->Value->Time.tm_mon << '.' << FileManager::Records.Head->Value->Time.tm_year << "\x1B[0m";
		Console::SetCursorPosition(m_XOffset + 50, m_YOffset + 17);
		std::cout << "|\n";

		std::cout << spaces + "|      Go there                                   |\n";
	}

	if (length > 1) {

		std::cout << spaces + "|                                                 |\n";
		std::cout << spaces + "|      \x1B[4mLast Record\x1B[0m                                |\n";
		std::cout << spaces + "|      Title: \x1B[3m" << FileManager::Records.Tail->Value->Title << "\x1B[0m";
		Console::SetCursorPosition(m_XOffset + 50, m_YOffset + 21);
		std::cout << "|\n";

		std::cout << spaces + "|      Date: \x1B[3m" << FileManager::Records.Tail->Value->Time.tm_mday << '.' << FileManager::Records.Tail->Value->Time.tm_mon << '.' << FileManager::Records.Tail->Value->Time.tm_year << "\x1B[0m";
		Console::SetCursorPosition(m_XOffset + 50, m_YOffset + 22);
		std::cout << "|\n";

		std::cout << spaces + "|      Go there                                   |\n";
	}

	std::cout << spaces + "|                                                 |\n";
	std::cout << spaces + "|                                                 |\n";
	std::cout << spaces + "---------------------------------------------------\n";
}

void Records::MoveTo(int index, bool up) {
	if (m_XOffset <= 0)
		throw std::exception("The console window is too small");

	int computedIndex = index - (m_YOffset + 9);
	int recordLength = FileManager::Records.Length();

	// Upper/Bottom limit
	if (computedIndex < 0 || computedIndex >(recordLength == 1 ? (9) : (14)))
		return;

	if (up) {
		Console::SetCursorPosition(m_XOffset + 4, index + 1);
		std::cout << "  ";

		int len = FileManager::Records.Length();
		if (computedIndex == 8)
		{
			if (len > 0)
				index -= 4;
			else
				index++;
		}
		else if (computedIndex == 13)
		{
			if (len > 1)
				index -= 4;
			else
				index++;
		}
	}
	else
	{
		Console::SetCursorPosition(m_XOffset + 4, index - 1);
		std::cout << "  ";

		int len = FileManager::Records.Length();
		if (computedIndex == 5)
		{
			if (len > 0)
				index += 4;
			else
				index--;
		}
		else if (computedIndex == 10)
		{
			if (len > 1)
				index += 4;
			else
				index--;
		}
	}

	Console::SetCursorPosition(m_XOffset + 4, index);
	std::cout << "->";

	m_CurrentItemIndex = index;
}

void Records::UserInputCompleted(int id, bool back)
{
	if (back)
	{
		MoveTo(m_CurrentItemIndex, true);
		return;
	}

	switch (id) {
		// Title
	case 0:
	{
		if (FileManager::Records.Head == nullptr)
			return;

		int index = 1;
		Node<Record>* current = FileManager::Records.Head;
		while (current->Next != nullptr) {
			int len = strlen(current->Value->Title) + 1;
			std::wstring wTitle(len, ' ');
			mbstowcs_s(0, &wTitle[0], len, current->Value->Title, len);

			if (wTitle.find(UserInput) != std::string::npos)
				break;

			current = current->Next;
			index++;
		}

		RecordDetail* detail = new RecordDetail();
		detail->ShowedRecord = current;
		detail->RecordPosition = index;
		Console::ChangeItem(detail, true);
		return;
	}

	// Position
	case 1:
	{
		if (UserInput.length() == 0)
			return;

		int position = std::stoi(UserInput) - 1;
		if (position < 0 || position > FileManager::Records.Length() - 1)
			return;

		RecordDetail* detail = new RecordDetail();
		detail->ShowedRecord = FileManager::Records.ElementAt(position);
		detail->RecordPosition = position + 1;
		Console::ChangeItem(detail, true);
		return;
	}

	// Date
	case 2:
	{
		int len = UserInput.length();
		if (len != 10)
			return;

		tm searchingTime;
		try {
			searchingTime.tm_mday = std::stoi(UserInput.substr(0, 2));
			searchingTime.tm_mon = std::stoi(UserInput.substr(3, 2));
			searchingTime.tm_year = std::stoi(UserInput.substr(6, 4));
		}
		catch (std::exception e) {
			return;
		}

		Node<Record>* current = FileManager::Records.Head;
		int position = 0;
		while (current != nullptr)
		{
			if (current->Value->Time.tm_mday == searchingTime.tm_mday &&
				current->Value->Time.tm_mon == searchingTime.tm_mon &&
				current->Value->Time.tm_year == searchingTime.tm_year)
				break;

			current = current->Next;
			position++;
		}

		if (current == nullptr)
			return;

		RecordDetail* detail = new RecordDetail();
		detail->ShowedRecord = current;
		detail->RecordPosition = position + 1;
		Console::ChangeItem(detail, true);
		return;
	}
	}
}

void Records::OnKeyInput(KEY_EVENT_RECORD* eventItem)
{
	if (!eventItem->bKeyDown)
		return;

	if (IsInputActive)
	{
		Renderable::OnKeyInput(eventItem);
		return;
	}

	switch (eventItem->wVirtualKeyCode) {
	case VK_DOWN:
		MoveTo(m_CurrentItemIndex + 1, false);
		break;

	case VK_UP:
		MoveTo(m_CurrentItemIndex - 1, true);
		break;

	case VK_BACK:
	case VK_ESCAPE:
		Console::ChangeItem(new MainMenu(), true);
		break;

	case VK_RETURN:
	case VK_RIGHT:
		switch (m_CurrentItemIndex - (m_YOffset + 9)) {
			// Create New
		case 0:
		{
			// TODO: Sort record by selected sorting method
			RecordDetail* detail = new RecordDetail();
			detail->RecordPosition = FileManager::Records.Length() + 1;
			detail->ShowedRecord = new Node<Record>();

			Record* record = new Record();
			int size = sizeof(u8"Nový Zápis\0");
		    char* title = (char*)malloc(size);
			strcpy_s(title, size, u8"Nový Zápis\0");

			record->Title = (const char*)title;
			record->Time.tm_mday = detail->RecordPosition;
			record->Time.tm_year = 2023;
			record->Time.tm_mon = 11;
			detail->ShowedRecord->Value = record;

			FileManager::Records.Append(detail->ShowedRecord);	
			Console::ChangeItem(detail, true);
			break;
		}

		// Find by Title
		case 1:
		{
			int length = FileManager::Records.Length();
			std::cout << (length == 0 ? "\x1B[31m" : "") << " Find by title: \x1B[0m";
			if (length != 0)
				ReadUserInput(1, 24, 0);
			else
				MoveTo(m_CurrentItemIndex, false);
			break;
		}

		// Go to Position
		case 2:
		{
			int length = FileManager::Records.Length();
			std::cout << (length == 0 ? "\x1B[31m" : "") << " Go to position: \x1B[0m";
			if (length != 0)
				ReadUserInput(1, length, 1, true);
			else
				MoveTo(m_CurrentItemIndex, false);
			break;
		}

		// Go to Date
		case 3:
		{
			int length = FileManager::Records.Length();
			std::cout << (length == 0 ? "\x1B[31m" : "") << " Go to date: \x1B[0m";
			if (length != 0)
				ReadUserInput(10, 10, 2);
			else
				MoveTo(m_CurrentItemIndex, false);
			break;
		}

		// Back
		case 4:
			Console::ChangeItem(new MainMenu(), true);
			break;

			// Go to Head
		case 9:
		{
			RecordDetail* details = new RecordDetail();
			details->ShowedRecord = FileManager::Records.Head;
			details->RecordPosition = 1;

			Console::ChangeItem(details, true);
			break;
		}

		// Go to Tail
		case 14:
		{
			RecordDetail* details = new RecordDetail();
			details->ShowedRecord = FileManager::Records.Tail;
			details->RecordPosition = FileManager::Records.Length();

			Console::ChangeItem(details, true);
			break;
		}

		}
		break;
	}
}

void Records::OnMouseInput(MOUSE_EVENT_RECORD* eventItem)
{

}

