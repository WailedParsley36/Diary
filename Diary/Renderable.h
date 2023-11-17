#pragma once
#include <Windows.h>

#include <string>

class Renderable
{
public:
	virtual void Render() = 0;

	virtual void OnKeyInput(KEY_EVENT_RECORD* eventItem);
	virtual void OnMouseInput(MOUSE_EVENT_RECORD* eventItem) {
		// Your definition
	}

	void ReadUserInput(int min, int max, int id, bool number = false, int xOffset = -1, bool verticalMovement = false, int verticalMax = 0);
	virtual void UserInputCompleted(int id, bool back = false) { };

protected:
	bool IsInputActive = false;
	std::wstring UserInput;

	void PrintCharacter(CHAR charToPrint);
	void PrintCharacter(WCHAR charToPrint);
	void PrintString(const CHAR* stringToPrint);
	void PrintString(const WCHAR* stringToPrint);

private:
	COORD m_ConsoleStartPos;
	int m_ID;
	int m_MinLimit;
	int m_MaxLimit;

	int m_VerticalMax;

	bool m_Vertical = true;
	bool m_IsNumber;

	int GetNewLineCount(bool fast = true);
	int GetNewLineIndex(int position = 0, int startIndex = 0, int* encounteredLines = nullptr);
	void GoToNewLine(int newLineIndex);

	bool CheckMaxLimit();
};
