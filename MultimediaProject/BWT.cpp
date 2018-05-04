#include "BWT.h"

BWT::BWT(string url):file(url,ios::binary)
{
	getText();
	prepareRotations();
	rle();
}

void BWT::getText()
{
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

void BWT::prepareRotations()
{
	rotations.push_back(inputText);

	//make rotations
	for (int i = 1; i < inputText.size(); i++) {
		wchar_t front = rotations[i-1].front();
		deque<wchar_t>temp = rotations[i - 1];
		temp.pop_front();
		temp.push_back(front);
		rotations.push_back(temp);
	}

	//sort rotations
	sort(rotations.begin(), rotations.end());
	
	//BWT output = encode the rotations "Last char of every rotation"
	for (int i = 0; i < rotations.size(); i++) {
		wchar_t charc = rotations[i][rotations[i].size()-1];
		bwtOutput+=charc;
		/*
		if (find(encodedString.begin(), encodedString.end(), charc) == encodedString.end()) {
			encodedString.push_back(charc);
			if(freqMap[charc]>1)
				encodedString.push_back(freqMap[charc]-1);
		}*/
	}
}

void BWT::rle()
{
	//encodedString = encode BWT output string with Run Length Encoding(RLE)
	string encodedString;
	unsigned char count = 0;
	for (int i = 0, j = 1; i < bwtOutput.size() - 1; i++, j++) {
		if (bwtOutput[i] == bwtOutput[j]) {
			count++;
			continue;
		}
		encodedString += bwtOutput[i];
		if (count >= 1) {
			encodedString += count;
		}
		count = 0;
	}
}

BWT::~BWT()
{
}
