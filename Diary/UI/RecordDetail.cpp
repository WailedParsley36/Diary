#include "RecordDetail.h"

#include "../Console.h"
#include "../FileManager.h"

#include "Records.h"
#include <iostream>

void RecordDetail::Render()
{
	m_XOffset = Console::BufferSize.X / 2 - 38;
	m_YOffset = Console::BufferSize.Y / 2 - 17;
	if (m_CurrentItemIndex == 0)
		m_CurrentItemIndex = m_YOffset + 26;

	Console::SetCursorPosition(0, m_YOffset);
	std::string spaces = std::string(m_XOffset, ' ');

	std::cout << spaces + "Main Menu -> Records[" << RecordPosition << "] -> " << ShowedRecord->Value->Title << '\n';
	std::cout << spaces + "-----------------------------------------------\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|\x1B[1m";
	std::cout << std::string(22 - ((strlen(ShowedRecord->Value->Title) + 13) / 2), ' ') << ShowedRecord->Value->Title << " - ";
	std::cout << ShowedRecord->Value->Time.tm_mday << '.' << ShowedRecord->Value->Time.tm_mon << '.' << ShowedRecord->Value->Time.tm_year << "\x1B[0m";
	Console::SetCursorPosition(m_XOffset + 46, m_YOffset + 3);
	std::cout << "|------------------------------------------------\n";

	std::cout << spaces + "|                                             |                                               |\n";
	std::cout << spaces + "|                                             |                   CONTROLS                    |\n";
	std::cout << spaces + "|                                             |                                               |\n";
	std::cout << spaces + "|                                             |     ESC -> Exit editing                       |\n";
	std::cout << spaces + "|                                             |     RETURN -> New line                        |\n";
	std::cout << spaces + "|                                             |     ARROWS -> Moving the cursor               |\n";
	std::cout << spaces + "|                                             |                                               |\n";
	std::cout << spaces + "|                                             |     PGUP -> Moves the page up                 |\n";
	std::cout << spaces + "|                                             |     PGDN -> Moves the page down               |\n";
	std::cout << spaces + "|                                             |                                               |\n";
	std::cout << spaces + "|                                             |                                               |\n";
	std::cout << spaces + "|                                             |------------------------------------------------\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                                             |\n";
	Console::SetCursorPosition(m_XOffset + 6, m_YOffset + 6);

	// TODO: Write the ShowedRecord->Value->Content on the page and take offset into consideration
	

	Console::SetCursorPosition(m_XOffset - spaces.length(), m_YOffset + 23);
	// TODO: Add handling to if we should write the ...
	std::cout << spaces + "|                     ...                     |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "-----------------------------------------------\n";
	std::cout << spaces + "|                    Save                     |\n";
	std::cout << spaces + "|                    Edit                     |\n";
	std::cout << spaces + "|                   Delete                    |\n";
	std::cout << spaces + "|                                             |\n";
	std::cout << spaces + "|                    " << (ShowedRecord->Next == nullptr ? "\x1B[31m" : "") << "Next\x1B[0m                     |\n";
	std::cout << spaces + "|                  " << (ShowedRecord->Previous == nullptr ? "\x1B[31m" : "") << "Previous\x1B[0m                   |\n";
	std::cout << spaces + "|                    Back                     |\n";
	std::cout << spaces + "-----------------------------------------------\n";

	MoveTo(m_CurrentItemIndex, true);
}

void RecordDetail::OnKeyInput(KEY_EVENT_RECORD* eventItem)
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
		Console::ChangeItem(new Records(), true);
		break;

	case VK_RIGHT:
	case VK_RETURN:
		switch (m_CurrentItemIndex - (m_YOffset + 26)) {
			// Save
		case 0:
			// TODO: Save edit changes and discard the edited if going back and not saving
			FileManager::SaveRecords();
			break;

			// Edit
		case 1:
			Console::SetCursorPosition(m_XOffset + 3, m_YOffset + 6);
			std::cout << "-> ";

			ReadUserInput(0, 35, 0, false, -1, true, 17);

			UserInput += L"";
			break;

			// Delete
		case 2:
		{
			int position = RecordPosition;

			Node<Record>* current = nullptr;
			if (ShowedRecord->Next != nullptr)
				current = ShowedRecord->Next;
			else if (ShowedRecord->Previous != nullptr)
			{
				current = ShowedRecord->Previous;
				position--;
			}

			FileManager::Records.RemoveAt(RecordPosition - 1);
			if (current == nullptr)
			{
				Console::ChangeItem(new Records(), true);
				return;
			}

			ShowedRecord = current;
			RecordPosition = position;
			Render();
			break;
		}

		// Next
		case 4:
			ShowedRecord = ShowedRecord->Next;
			RecordPosition++;
			Render();
			break;

			// Previous
		case 5:
			ShowedRecord = ShowedRecord->Previous;
			RecordPosition--;
			Render();
			break;

			// Back
		case 6:
			FileManager::SaveRecords();
			Console::ChangeItem(new Records(), true);
			break;
		}
		break;
	}

}

void RecordDetail::OnMouseInput(MOUSE_EVENT_RECORD* eventItem)
{

}

void RecordDetail::UserInputCompleted(int id, bool back)
{
	Console::SetCursorPosition(m_XOffset + 3, m_YOffset + 6);
	std::cout << "   ";
}

int RecordDetail::GetXOffset(int index) {
	switch (index) {
	case 6:
	case 4:
	case 1:
	case 0:
		return 18;

	case 2:
		return 17;

	case 5:
		return 16;

	default:
		return 1;
	}
}
void RecordDetail::MoveTo(int index, bool up)
{
	if (m_XOffset <= 0)
		throw std::exception("The console window is too small");

	int computedIndex = index - (m_YOffset + 26);

	// Upper/Bottom limit
	if (computedIndex > 6 || computedIndex < 0)
		return;

	Console::SetCursorPosition(m_XOffset + GetXOffset(computedIndex + (up ? 1 : -1)), index + (up ? 1 : -1));
	std::cout << "  ";

	if ((up ? computedIndex == 4 : computedIndex == 3) && ShowedRecord->Next == nullptr)
	{
		MoveTo(up ? index - 1 : index + 2, up);
		return;
	}
	else if (computedIndex == 5 && ShowedRecord->Previous == nullptr)
	{
		MoveTo(up ? index - 1 : index + 1, up);
		return;
	}

	if (up && computedIndex == 3) {
		computedIndex--;
		index--;
	}
	else if (computedIndex == 3) {
		computedIndex++;
		index++;
	}

	Console::SetCursorPosition(m_XOffset + GetXOffset(computedIndex), index);
	std::cout << "->";

	m_CurrentItemIndex = index;
}