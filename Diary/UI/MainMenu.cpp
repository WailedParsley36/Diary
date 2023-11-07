#include "../FileManager.h"

#include "MainMenu.h"
#include "Settings.h"
#include "Records.h"

#include <iostream>

void MainMenu::Render()
{
	m_XOffset = Console::BufferSize.X / 2 - 24;
	m_YOffset = Console::BufferSize.Y / 2 - 7;
	m_CurrentItemIndex = m_YOffset + 7;

	Console::SetCursorPosition(0, m_YOffset);
	std::string spaces = std::string(m_XOffset, ' ');

	// Y = 13
	// X = 48
	std::cout << spaces + "Main Menu\n";
	std::cout << spaces + "------------------------------------------------\n";
	std::cout << spaces + "|                                              |\n";
	std::cout << spaces + "|\x1B[1m";
	std::cout << std::string(24 - (strlen(Console::Settings->Title) / 2), ' ') << Console::Settings->Title << "\x1B[0m";
	Console::SetCursorPosition(m_XOffset + 47, m_YOffset + 3);
	std::cout << "|\n";

	std::cout << spaces + "|                                              |\n";
	std::cout << spaces + "|                                              |\n";
	std::cout << spaces + "|              -> Show Records                 |\n";
	std::cout << spaces + "|                   Settings                   |\n";
	std::cout << spaces + "|                     Exit                     |\n";
	std::cout << spaces + "|                                              |\n";
	std::cout << spaces + "|                                              |\n";
	std::cout << spaces + "|              You have " << FileManager::Records.Length() << " entries";
	Console::SetCursorPosition(m_XOffset + 47, m_YOffset + 11);
	std::cout << "|\n";

	std::cout << spaces + "|                                              |\n";
	std::cout << spaces + "------------------------------------------------\n";
}

void MainMenu::MoveTo(int index)
{
	int computedIndex = index - (m_YOffset + 7);
	switch (computedIndex)
	{
	case 0:
		Console::SetCursorPosition(m_XOffset + 17, m_YOffset + 7);
		std::cout << "   ";

		Console::SetCursorPosition(m_XOffset + 15, m_YOffset + 6);
		std::cout << "->";
		break;

	case 1:
		Console::SetCursorPosition(m_XOffset + 15, m_YOffset + 6);
		std::cout << "   ";
		Console::SetCursorPosition(m_XOffset + 19, m_YOffset + 8);
		std::cout << "   ";

		Console::SetCursorPosition(m_XOffset + 17, m_YOffset + 7);
		std::cout << "->";
		break;

	case 2:
		Console::SetCursorPosition(m_XOffset + 17, m_YOffset + 7);
		std::cout << "   ";

		Console::SetCursorPosition(m_XOffset + 19, m_YOffset + 8);
		std::cout << "->";
		break;
	}
	if (computedIndex == 3)
		index--;
	else if (computedIndex == -1)
		index++;

	m_CurrentItemIndex = index;
}

void MainMenu::OnKeyInput(KEY_EVENT_RECORD* eventItem)
{
	if (!eventItem->bKeyDown)
		return;

	switch (eventItem->wVirtualKeyCode) {
	case VK_DOWN:
		MoveTo(m_CurrentItemIndex + 1);
		break;

	case VK_UP:
		MoveTo(m_CurrentItemIndex - 1);
		break;

	case VK_RETURN:
	case VK_RIGHT:
		switch (m_CurrentItemIndex - (m_YOffset + 7)) {
			// Show Records
		case 0:
			Console::ChangeItem(new Records(), true);
			break;

			// Settings
		case 1:
			Console::ChangeItem(new Settings(), true);
			break;

			// Exit
		case 2:
			Console::Exit(0);
		}
		break;
	}
}

void MainMenu::OnMouseInput(MOUSE_EVENT_RECORD* eventItem)
{

}

