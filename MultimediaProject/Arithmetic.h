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

struct Chararcter{
	wchar_t chararcter;
	double lowerBound;
	double upperBound;
};

class Arithmetic
{
	deque<wchar_t> inputText;
	vector<Chararcter*>ranges;
	map<wchar_t, double>freqMap;
	string fileName;
	string bwtOutput;
	wifstream file;

	void getText();
	void constructProbabilityLine();
	void sortLastElement();

public:
	Arithmetic(string url);
	~Arithmetic();
};

