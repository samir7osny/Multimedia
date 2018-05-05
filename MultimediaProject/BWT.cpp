#include "BWT.h"


BWT::BWT()
{

}

BWT::BWT(deque<wchar_t> input)
{

}

deque<wchar_t> BWT::encode(deque<wchar_t> inputText)
{
	deque<wchar_t> bwtOutput;
	//flag to know the last element
	inputText.push_back(0);

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
		bwtOutput.push_back(charc);
		/*
		if (find(encodedString.begin(), encodedString.end(), charc) == encodedString.end()) {
			encodedString.push_back(charc);
			if(freqMap[charc]>1)
				encodedString.push_back(freqMap[charc]-1);
		}*/
	}

	return bwtOutput;
}

deque<wchar_t> BWT::decode(deque<wchar_t> inputText)
{
	//bwtOutput
	deque<wchar_t> bwtOutputSorted = inputText;
	sort(bwtOutputSorted.begin(), bwtOutputSorted.end());

	deque<int> indexMap;
	map<wchar_t, int> charFreq;
	for (int i = 0; i < inputText.size(); i++)
	{
		int tempIndex = charFreq[inputText[i]]++;
		for (int j = 0; j < bwtOutputSorted.size(); j++)
		{
			if (bwtOutputSorted[j] == inputText[i])
			{
				indexMap.push_back(j + tempIndex);
				break;
			}
		}
	}

	deque<wchar_t> result;
	int currentIndex = 0;
	wchar_t temp = inputText[currentIndex];
	while (temp != '\0')
	{
		result.push_front(temp);
		currentIndex = indexMap[currentIndex];
		temp = inputText[currentIndex];
	}

	/*wofstream decompressedFile("BWTOutputDe.txt", ios::out | ios::binary);
	decompressedFile.clear();
	decompressedFile.imbue(std::locale(decompressedFile.getloc(),
		new std::codecvt_utf8<wchar_t, 0xfff, std::consume_header>));
	for (int i = 0; i < result.size(); i++)
	{
		decompressedFile << result[i];
	}
	decompressedFile.close();

	decompressedFile.open("BWTOutput.txt", ios::out | ios::binary);
	decompressedFile.clear();
	decompressedFile.imbue(std::locale(decompressedFile.getloc(),
		new std::codecvt_utf8<wchar_t, 0xfff, std::consume_header>));
	for (int i = 0; i < bwtOutput.size(); i++)
	{
		decompressedFile << bwtOutput[i];
	}
	decompressedFile.close();*/

	return result;
}

BWT::~BWT()
{
}
