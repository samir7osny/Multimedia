#include "Arithmetic.h"

Arithmetic::Arithmetic(string url):file(url,ios::binary){
	getText();
	constructProbabilityLine();
}


void Arithmetic::sortMap(map<wchar_t, double>::iterator it1, map<wchar_t, double>::iterator it2) {
	// Declaring a set that will store the pairs using above comparision logic
	sortedFreqMap= new set<pair<wchar_t, double>, Comparator>(freqMap.begin(), freqMap.end(), compFunctor);
}

void Arithmetic::getText() {
	// apply BOM-sensitive UTF-16 facet
	file.imbue(std::locale(file.getloc(),
		new std::codecvt_utf8<wchar_t, 0xfff, std::consume_header>));
	// read     
	for (wchar_t c; file.get(c); ) {
		//std::cout << std::showbase << std::hex << c << '\n';
		inputText.push_back(c);
		freqMap[c]++;
	}
}

void Arithmetic::constructProbabilityLine()
{
	//Calculate probabilty for ezch character
	for (map<wchar_t, double>::iterator it = freqMap.begin(); it != freqMap.end(); it++)
		it->second = it->second / inputText.size();
	
	//Sort freqMap to build probability line
	sortMap(freqMap.begin(), freqMap.end());

	double currentLower = 0;

	for (map<wchar_t, double>::iterator it = freqMap.begin(); it != freqMap.end(); it++) {
		Range*newRange = new Range();
		newRange->chararcter = it->first;
		newRange->lowerBound = currentLower;
		newRange->probability = it->second;
		currentLower += it->second;
		ranges.push_back(newRange);
	}

}

Arithmetic::~Arithmetic()
{
}
