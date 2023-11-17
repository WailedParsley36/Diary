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
		int len = UserInput.length() - GetNewLineCount();
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
			PrintString(UserInput.c_str());

			std::cout << ' ';
			Console::SetCursorPosition(currentConsolePos.X - 1, currentConsolePos.Y);
			Console::SetCursorVisibility(true);
		}
	}
	else if (eventItem->wVirtualKeyCode == VK_RETURN) {
		if (m_IsNumber) {
			int value = std::stoi(UserInput);
			if (value < m_MinLimit || value > m_MaxLimit)
				return;
		}
		else if (m_Vertical) {
			// TODO: Move down by 1
			GoToNewLine(GetNewLineIndex());
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
		if (!m_Vertical)
			UserInput.clear();

		UserInputCompleted(m_ID, true);
	}
	else if (m_Vertical && (eventItem->wVirtualKeyCode == VK_DOWN || eventItem->wVirtualKeyCode == VK_UP)) {
		COORD currentConsolePos = Console::GetCursorPosition();

		if (eventItem->wVirtualKeyCode == VK_UP)
		{
			int computed = (currentConsolePos.Y - 1) - m_ConsoleStartPos.Y;
			if (computed < m_VerticalMax && m_ConsoleStartPos.Y + computed >= m_ConsoleStartPos.Y)
				GoToNewLine(computed);
		}
		else if (eventItem->wVirtualKeyCode == VK_DOWN) {
			int computed = (currentConsolePos.Y + 1) - m_ConsoleStartPos.Y;
			if (computed < m_VerticalMax && m_ConsoleStartPos.Y + computed >= m_ConsoleStartPos.Y)
				GoToNewLine(computed);
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

		// TODO: Vertical check max limit of all of the 
		int len = UserInput.length() - GetNewLineCount();
		if (!m_Vertical && len + 1 > m_MaxLimit - 2)
			return;

		int position = currentConsolePos.X - m_ConsoleStartPos.X;
		if (m_Vertical) {
			if (!CheckMaxLimit())
				return;

			int yOffset = currentConsolePos.Y - m_ConsoleStartPos.Y;
			UserInput.insert(GetNewLineIndex(yOffset), 1, eventItem->uChar.UnicodeChar);
		}
		else
		{
			if (len + 1 == position)
				UserInput += eventItem->uChar.UnicodeChar;
			else
				UserInput.insert(position - 1, 1, eventItem->uChar.UnicodeChar);
		}

		len++;

		if (m_IsNumber) {

			int number = std::stoi(UserInput);
			if (number > m_MaxLimit)
			{
				UserInput.pop_back();
				return;
			}
		}

		if (m_Vertical) {
			Console::SetCursorPosition(m_ConsoleStartPos.X + 1, m_ConsoleStartPos.Y);
			PrintString(UserInput.c_str());

			Console::SetCursorPosition(currentConsolePos.X + 1, currentConsolePos.Y);
		}
		else if (position == len)
			PrintCharacter(eventItem->uChar.UnicodeChar);
		else {
			Console::SetCursorVisibility(false);

			Console::SetCursorPosition(m_ConsoleStartPos.X + 1, currentConsolePos.Y);
			for (int i = 0; i < len; i++) {
				PrintCharacter(UserInput[i]);
			}

			Console::SetCursorPosition(currentConsolePos.X + 1, currentConsolePos.Y);
			Console::SetCursorVisibility(true);
		}
	}
}

void Renderable::ReadUserInput(int min, int max, int id, bool number, int xOffset, bool verticalMovement, int verticalMax) {
	IsInputActive = true;
	m_ConsoleStartPos = Console::GetCursorPosition();
	m_ConsoleStartPos.X += xOffset;
	UserInput.clear();

	m_MaxLimit = max;
	if (!number)
		m_MaxLimit += 2;

	m_MinLimit = min;
	m_ID = id;
	m_IsNumber = number;

	m_Vertical = verticalMovement;
	if (m_Vertical) {
		m_VerticalMax = verticalMax;
		UserInput = std::wstring(verticalMax, L'\n');
	}
	Console::SetCursorVisibility(true);
}

void Renderable::PrintCharacter(CHAR charToPrint)
{
	WCHAR converted = std::btowc(charToPrint);
	PrintCharacter(converted);
}
void Renderable::PrintCharacter(WCHAR charToPrint)
{
	char utf8_character[5];
	WideCharToMultiByte(CP_UTF8, 0, &charToPrint, 1, utf8_character, 4, NULL, NULL);
	for (int i = 0; i < 5; i++)
	{
		if (utf8_character[i] == -52)
		{
			utf8_character[i] = '\0';
			break;
		}
	}

	printf("%s", &utf8_character);
}
void Renderable::PrintString(const CHAR* stringToPrint)
{
	int len = strlen(stringToPrint);
	for (int i = 0; i < len; i++)
	{
		if (m_Vertical && *(stringToPrint + i) == '\n')
		{
			COORD position = Console::GetCursorPosition();
			Console::SetCursorPosition(m_ConsoleStartPos.X + 1, position.Y + 1);
		}
		else
			PrintCharacter(*(stringToPrint + i));
	}
}
void Renderable::PrintString(const WCHAR* stringToPrint)
{
	if (stringToPrint == nullptr)
		return;

	int len = wcslen(stringToPrint);
	for (int i = 0; i < len; i++)
	{
		if (m_Vertical && *(stringToPrint + i) == '\n')
		{
			COORD position = Console::GetCursorPosition();
			Console::SetCursorPosition(m_ConsoleStartPos.X + 1, position.Y + 1);
		}
		else
			PrintCharacter(*(stringToPrint + i));
	}
}

int Renderable::GetNewLineCount(bool fast)
{
	if (fast)
		return m_VerticalMax;

	int count = 0;
	int len = UserInput.length();
	for (int i = 0; i < len; i++)
		if (UserInput[i] == '\n')
			count++;

	return count;
}
int Renderable::GetNewLineIndex(int position, int startIndex, int* encounteredLines)
{
	int result = -1;
	int len = UserInput.length();
	for (int i = startIndex; i < len; i++)
	{
		if (UserInput[i] == '\n')
		{
			result++;
			if (position == result) {
				if (encounteredLines != nullptr)
					*encounteredLines = result;

				result = i;
				break;
			}
		}
	}

	return result;
}
void Renderable::GoToNewLine(int newLineIndex)
{
	if (newLineIndex == 0)
	{
		Console::SetCursorPosition(m_ConsoleStartPos.X + 1, m_ConsoleStartPos.Y);
		return;
	}


	// Finding the index of the newline we are searching for
	int len = UserInput.length();
	int count = 0;
	newLineIndex = GetNewLineIndex(newLineIndex, 0, &count);

	// The newline index doesn't exist
	if (newLineIndex == -1)
	{
		// TODO: Create new newline or just go to the 
		return;
	}

	// We found the index of the newline
	COORD currentConsolePos = Console::GetCursorPosition();

	// Length of current line
	for (int i = newLineIndex; i < len; i++)
	{
		if (UserInput[i] == '\n')
			len = i - newLineIndex;
	}

	// If the cursor is in higher position than the text in this line has we need to move to the end
	if (currentConsolePos.X - (m_ConsoleStartPos.X + 1) > len) {
		Console::SetCursorPosition(m_ConsoleStartPos.X + 1 + len, currentConsolePos.Y + count);
	}
	// Normal moving
	else
		Console::SetCursorPosition(currentConsolePos.X, m_ConsoleStartPos.Y + count);
}

bool Renderable::CheckMaxLimit()
{
	if (m_Vertical) {
		int currentLength = 0;
		int length = UserInput.length();
		for (int i = 0; i < length; i++) {
			if (UserInput[i] == '\n') {
				if (currentLength > m_MaxLimit)
					return false;

				currentLength = 0;
			}
			else
				currentLength++;
		}
	}

	return true;
}
