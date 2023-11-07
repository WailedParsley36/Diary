#pragma once
#include "..\Console.h"

class Settings : public Renderable
{
public:
	virtual void Render() override;
	virtual void OnKeyInput(KEY_EVENT_RECORD* eventItem);
	virtual void OnMouseInput(MOUSE_EVENT_RECORD* eventItem);

private:
	ConsoleStartupInfo* m_Settings = new ConsoleStartupInfo();
	int m_CurrentItemIndex = 7;
	int m_XOffset;
	bool m_IsSettingValue = false;

	void MoveTo(int index, bool selected = false);
	void TakeAction(bool up);
};

