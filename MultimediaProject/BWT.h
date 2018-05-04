#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <deque>
#include <map>
#include <locale>
#include <queue>
#include <codecvt>
#include <algorithm>
using namespace std;

class BWT
{
	deque<wchar_t> inputText;
	vector<deque<wchar_t>>rotations;
	map<wchar_t, int>freqMap;
	string fileName;
	string bwtOutput;
	wifstream file;

public:
	BWT(string url);
	void getText();
	void prepareRotations();
	void rle();
	~BWT();
};