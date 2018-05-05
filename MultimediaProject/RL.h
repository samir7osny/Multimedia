#pragma once
#include "Headers.h"

class RL
{
public:
	RL();
	deque<wchar_t> encode(deque<wchar_t> inputText);
	deque<wchar_t> decode(deque<wchar_t> inputText);
	~RL();
};

