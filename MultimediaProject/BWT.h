#pragma once
#include "Headers.h"

class BWT
{
	vector<deque<wchar_t>>rotations;
	int binarySearch(deque<wchar_t>* dequeSorted, int l, int r, wchar_t x);
public:
	BWT();
	BWT(deque<wchar_t> input);
	deque<wchar_t> encode(deque<wchar_t> inputText);
	deque<wchar_t> decode(deque<wchar_t> inputText);
	~BWT();
};

