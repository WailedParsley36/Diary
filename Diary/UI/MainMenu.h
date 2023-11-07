#pragma once
#include "..\Renderable.h"

class MainMenu : public Renderable {
public:
	virtual void Render() override;
	virtual void OnKeyInput(KEY_EVENT_RECORD* eventItem);
	virtual void OnMouseInput(MOUSE_EVENT_RECORD* eventItem);

private:
	int m_CurrentItemIndex;
	int m_XOffset;
	int m_YOffset;

	void MoveTo(int index);
};