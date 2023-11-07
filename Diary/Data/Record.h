#pragma once
#include <ctime>

struct Record
{
public:
	const char* Title;
	const char* Content;
	tm Time;

	inline ~Record() {
		delete Title;

		if (Content != nullptr)
			delete Content;
	}
};