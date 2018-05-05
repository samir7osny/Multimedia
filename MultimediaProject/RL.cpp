#include "RL.h"



RL::RL()
{
}

deque<wchar_t> RL::encode(deque<wchar_t> inputText)
{
	deque<wchar_t> Output;
	int counter = 0;
	for (int i = 0; i < inputText.size(); i++)
	{
		if (inputText[i] == 0)
		{
			counter++;
		}
		else if (counter >= 1)
		{
			Output.push_back(counter);
			Output.push_back(inputText[i]);
			counter = 1;
		}
		else
		{
			Output.push_back(inputText[i]);
			counter = 1;
		}
	}
	Output.push_back(counter);
	return Output;
}


deque<wchar_t> RL::decode(deque<wchar_t> inputText)
{
	deque<wchar_t> Output;
	for (int i = 0; i < inputText.size(); i+=2)
	{
		Output.push_back(inputText[i]);
		for (int j = 0; j < inputText[i+1] - 1; j++)
		{
			Output.push_back((wchar_t)0);
		}
	}
	return Output;
}

RL::~RL()
{
}
