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

private:
	COORD m_ConsoleStartPos;
	int m_ID;
	int m_MinLimit;
	int m_MaxLimit;

	int m_VerticalMax;

	bool m_Vertical;
	bool m_IsNumber;
};
