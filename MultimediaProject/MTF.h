#pragma once
#include "Headers.h"


class MTF
{
public:
	MTF();
	MTF(deque<wchar_t> input, deque<wchar_t> charList);
	deque<wchar_t> encode(deque<wchar_t> inputText, deque<wchar_t> charList, bool fromBWT);
	deque<wchar_t> decode(deque<wchar_t> inputText, deque<wchar_t> charList, bool fromBWT);
	int getCharIndex(deque<wchar_t> &charList, wchar_t c);
	~MTF();
};

