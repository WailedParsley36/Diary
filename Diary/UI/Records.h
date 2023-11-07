#pragma once
#include "..\Renderable.h"

class Records : public Renderable
{
public:
	virtual void Render() override;
	virtual void OnKeyInput(KEY_EVENT_RECORD* eventItem) override;
	virtual void OnMouseInput(MOUSE_EVENT_RECORD* eventItem) override;

	virtual void UserInputCompleted(int id, bool back = false) override;

private:
	int m_XOffset;
	int m_YOffset;
	int m_CurrentItemIndex;

	void MoveTo(int index, bool up);
};