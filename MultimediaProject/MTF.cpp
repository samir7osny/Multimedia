#include "MTF.h"



MTF::MTF(deque<wchar_t> input, deque<wchar_t> charList)
{

}
MTF::MTF()
{

}

deque<wchar_t> MTF::encode(deque<wchar_t> inputText, deque<wchar_t> charList,bool fromBWT)
{
	if (fromBWT)
		charList.push_back('\0');
	deque<wchar_t> Output;
	sort(charList.begin(), charList.end());
	for (int i = 0; i < inputText.size(); i++)
	{
		wchar_t temp = inputText[i];
		int index = getCharIndex(charList,inputText[i]);
		charList.erase(charList.begin() + index);
		charList.push_front(temp);
		Output.push_back((wchar_t)index);
	}
	return Output;
}

deque<wchar_t> MTF::decode(deque<wchar_t> inputText, deque<wchar_t> charList, bool fromBWT)
{
	if (fromBWT)
		charList.push_back('\0');
	deque<wchar_t> Output;
	sort(charList.begin(), charList.end());
	for (int i = 0; i < inputText.size(); i++)
	{
		int index = inputText[i];
		wchar_t temp = charList[index];
		charList.erase(charList.begin() + index);
		charList.push_front(temp);
		Output.push_back(temp);
	}
	return Output;
}

int MTF::getCharIndex(deque<wchar_t> &charList, wchar_t c)
{
	for (int i = 0; i < charList.size(); i++)
	{
		if (charList[i] == c)
		{
			return i;
		}
	}
	return -1;
}

MTF::~MTF()
{
}
