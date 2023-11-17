#pragma once
#include <ctime>

struct Record
{
public:
	const CHAR* Title;
	const WCHAR* Content;
	tm Time;

	inline ~Record() {
		delete Title;

		if (Content != nullptr)
			delete Content;
	}
};