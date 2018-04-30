#include "Arithmetic.h"



Arithmetic::Arithmetic(string url):file(url,ios::binary){
	getText();
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

	//Calculate probabilty for ezch character
	for (map<wchar_t,double>::iterator it = freqMap.begin(); it != freqMap.end(); it++)
		it->second = it->second / inputText.size();

}

void Arithmetic::constructProbabilityLine()
{
}

Arithmetic::~Arithmetic()
{
}
