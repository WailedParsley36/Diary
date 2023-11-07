#include "Renderable.h"

#include <iostream>
#include "Console.h"

void Renderable::OnKeyInput(KEY_EVENT_RECORD* eventItem) {
	if (!IsInputActive || !eventItem->bKeyDown)
		return;

	// Implementation for ReadUserInput to work

	// Removing last character and moving cursor back
	if (eventItem->wVirtualKeyCode == VK_BACK)
	{
		COORD currentConsolePos = Console::GetCursorPosition();
		if (m_ConsoleStartPos.X >= currentConsolePos.X - 1)
			return;

		Console::SetCursorVisibility(false);
		int len = UserInput.length();
		if (len != 0)
		{
			int i = (currentConsolePos.X - 2) - m_ConsoleStartPos.X;
			if (i > len - 1)
			{
				Console::SetCursorPosition(currentConsolePos.X - 1, currentConsolePos.Y);
				Console::SetCursorVisibility(true);
				return;
			}

			for (; i < len - 1; i++)
				UserInput[i] = UserInput[i + 1];

			UserInput.pop_back();
			Console::SetCursorPosition(m_ConsoleStartPos.X + 1, currentConsolePos.Y);
			std::wcout << UserInput << ' ';
			Console::SetCursorPosition(currentConsolePos.X - 1, currentConsolePos.Y);
		}
		else
		{
			std::cout << ' ';
			Console::SetCursorPosition(currentConsolePos.X - 1, currentConsolePos.Y);
		}

		Console::SetCursorVisibility(true);
	}
	else if (eventItem->wVirtualKeyCode == VK_RETURN) {
		if (m_IsNumber) {
			int value = std::stoi(UserInput);
			if (value < m_MinLimit || value > m_MaxLimit)
				return;
		}
		else {
			int len = UserInput.length();
			if (len > m_MaxLimit && len < m_MinLimit)
				return;
		}

		Console::SetCursorVisibility(false);
		IsInputActive = false;
		UserInputCompleted(m_ID);
	}
	else if (eventItem->wVirtualKeyCode == VK_LEFT)
	{
		COORD currentConsolePos = Console::GetCursorPosition();
		if (m_ConsoleStartPos.X >= currentConsolePos.X - 1)
			return;

		Console::SetCursorPosition(currentConsolePos.X - 1, currentConsolePos.Y);
	}
	else if (eventItem->wVirtualKeyCode == VK_RIGHT)
	{
		COORD currentConsolePos = Console::GetCursorPosition();
		if (m_ConsoleStartPos.X + (m_IsNumber ? std::to_string(m_MaxLimit).length() + 1 : m_MaxLimit) <= currentConsolePos.X + 1)
			return;

		Console::SetCursorPosition(currentConsolePos.X + 1, currentConsolePos.Y);
	}
	else if (eventItem->wVirtualKeyCode == VK_ESCAPE)
	{
		Console::SetCursorVisibility(false);
		IsInputActive = false;
		UserInput.clear();

		UserInputCompleted(m_ID, true);
	}
	else if (m_Vertical && (eventItem->wVirtualKeyCode == VK_DOWN || eventItem->wVirtualKeyCode == VK_UP)) {
		COORD currentConsolePos = Console::GetCursorPosition();

		if (eventItem->wVirtualKeyCode == VK_UP)
		{
			int computed = (currentConsolePos.Y - 1) - m_ConsoleStartPos.Y;
			if (computed < m_VerticalMax && m_ConsoleStartPos.Y + computed >= m_ConsoleStartPos.Y)
				Console::SetCursorPosition(currentConsolePos.X, currentConsolePos.Y - 1);
		}
		else if (eventItem->wVirtualKeyCode == VK_DOWN) {
			int computed = (currentConsolePos.Y + 1) - m_ConsoleStartPos.Y;
			if (computed < m_VerticalMax && m_ConsoleStartPos.Y + computed >= m_ConsoleStartPos.Y)
				Console::SetCursorPosition(currentConsolePos.X, currentConsolePos.Y + 1);
		}
	}
	else if (eventItem->uChar.AsciiChar != '\0' || eventItem->uChar.UnicodeChar != L'\0') {
		if (m_IsNumber && (eventItem->uChar.AsciiChar < 49 || eventItem->uChar.AsciiChar > 57))
			return;

		COORD currentConsolePos = Console::GetCursorPosition();
		if (!m_IsNumber && m_ConsoleStartPos.X + m_MaxLimit <= currentConsolePos.X + 1)
		{
			if (m_Vertical && currentConsolePos.Y + 1 > m_ConsoleStartPos.Y + m_VerticalMax)
			{
				currentConsolePos.X = m_ConsoleStartPos.X;
				currentConsolePos.Y = currentConsolePos.Y + 1;
				Console::SetCursorPosition(currentConsolePos.X, currentConsolePos.Y);
				UserInput += L'\n';
			}
			else
				return;
		}

		// TODO: Insert to position based on start position
		// TODO: Vertical -> Insert to position based on start position and vertical position \n
		UserInput += eventItem->uChar.UnicodeChar;

		if (m_IsNumber) {

			int number = std::stoi(UserInput);
			if (number > m_MaxLimit)
			{
				UserInput.pop_back();
				return;
			}
		}

		if (eventItem->uChar.AsciiChar < 0)
		{
			char utf8_character[5];
			WideCharToMultiByte(CP_UTF8, 0, &eventItem->uChar.UnicodeChar, 1, utf8_character, 4, NULL, NULL);
			utf8_character[4] = '\0';
			utf8_character[3] = '\0';

			printf("%s", &eventItem->uChar.AsciiChar);
		}
		else
			printf("%c", eventItem->uChar.UnicodeChar);
	}
}

void Renderable::ReadUserInput(int min, int max, int id, bool number, int xOffset, bool verticalMovement, int verticalMax) {
	IsInputActive = true;
	m_ConsoleStartPos = Console::GetCursorPosition();
	m_ConsoleStartPos.X += xOffset;
	UserInput.clear();

	m_VerticalMax = verticalMax;
	m_MaxLimit = max;
	if (!number)
		m_MaxLimit += 2;

	m_MinLimit = min;
	m_ID = id;
	m_IsNumber = number;

	m_Vertical = verticalMovement;
	Console::SetCursorVisibility(true);
}