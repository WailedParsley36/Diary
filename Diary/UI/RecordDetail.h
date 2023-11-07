#pragma once

#include "../Renderable.h"

#include "../Data/Record.h"
#include "../Data/Node.h"

class RecordDetail : public Renderable
{
public:
	virtual void Render() override;
	virtual void OnKeyInput(KEY_EVENT_RECORD* eventItem);
	virtual void OnMouseInput(MOUSE_EVENT_RECORD* eventItem);

	virtual void UserInputCompleted(int id, bool back = false) override;

	int RecordPosition;
	Node<Record>* ShowedRecord;
private:
	int m_XOffset;
	int m_YOffset;
	int m_CurrentItemIndex;

	int m_PageOffset;

	int GetXOffset(int index);
	void MoveTo(int index, bool up);
};