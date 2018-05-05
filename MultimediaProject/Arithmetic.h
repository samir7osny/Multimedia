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
#include <functional>
#include <set>
using namespace std;

struct Range{
	wchar_t chararcter;
	double lowerBound;
	double probability;
};


class Arithmetic
{

	// Declaring the type of Predicate that accepts 2 pairs and return a bool
	typedef function<bool(pair<wchar_t, double>, pair<wchar_t, double>)> Comparator;

	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
	Comparator compFunctor = [](pair<wchar_t, double> elem1, pair<wchar_t, double> elem2)
	{
		return elem1.second < elem2.second;
	};

	deque<wchar_t> inputText;
	vector<Range*>ranges;
	map<wchar_t, double>freqMap;
	string fileName;
	string bwtOutput;
	wifstream file;
	set<pair<wchar_t, double>, Comparator>*sortedFreqMap;

	void getText();
	void constructProbabilityLine();
	void sortMap(map<wchar_t, double>::iterator it1, map<wchar_t, double>::iterator it2);

public:
	Arithmetic(string url);
	~Arithmetic();
};