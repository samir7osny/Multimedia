#include "BWT.h"


BWT::BWT()
{

}

BWT::BWT(deque<wchar_t> input)
{

}

deque<wchar_t> BWT::encode(deque<wchar_t> inputText)
{
	struct Local {
		Local(deque<wchar_t>* arr, int size) { this->arr = arr; this->size = size; }
		bool operator () (int li, int ri) {
			int ac = size;
			//cout << (int)(*arr)[li] << " " << (int)(*arr)[ri] << "=======";
			while ((*arr)[li] == (*arr)[ri])
			{
				//cout << "0";
				if (++li == size)
					li = 0;
				if (++ri == size)
					ri = 0;
				if (--ac <= 0)
					return false;
			}
			if ((*arr)[li] < (*arr)[ri])
			{
				//cout << "1" << endl;
				return true;
			}
			//cout << "0" << endl;
			return false;
		}

		deque<wchar_t>* arr;
		int size;
	};

	inputText.push_back(0);

	deque<wchar_t> bwtOutput;
	bwtOutput.resize(inputText.size());
	//flag to know the last element
	int* indices = new int[inputText.size()];
	for (int i = 0; i < inputText.size(); i++)
	{
		indices[i] = i;
	}
	sort(indices, indices + inputText.size(), Local(&inputText, inputText.size()));

	
	//BWT output = encode the rotations "Last char of every rotation"
	for (int i = 0; i < inputText.size(); i++) {
		bwtOutput[i] = inputText[(indices[i] + inputText.size() - 1) % inputText.size()];
	}

	return bwtOutput;
}

/*int BWT::binarySearch(deque<wchar_t>* dequeSorted, wchar_t c)
{
	int i = 0, j = 0;
}*/
int BWT::binarySearch(deque<wchar_t>* dequeSorted, int l, int r, wchar_t x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		// If the element is present at the middle 
		// itself
		if ((*dequeSorted)[mid] == x)
		{
			if (mid - 1 > 0 && (*dequeSorted)[mid-1] == x)
			{
				return binarySearch(dequeSorted, l, mid - 1, x);
			}
			return mid;
		}

		// If element is smaller than mid, then 
		// it can only be present in left subarray
		if ((*dequeSorted)[mid] > x)
			return binarySearch(dequeSorted, l, mid - 1, x);

		// Else the element can only be present
		// in right subarray
		return binarySearch(dequeSorted, mid + 1, r, x);
	}

	// We reach here when element is not 
	// present in array
	return -1;
}

deque<wchar_t> BWT::decode(deque<wchar_t> inputText)
{
	/*struct Local {
		Local(int* arr) { this->arr = arr; }
		bool operator () (const pair<wchar_t, int> &li, const pair<wchar_t, int> &ri) {
			if (li.first < ri.first)
			{
				int temp = arr[li.second];
				arr[li.second] = arr[ri.second];
				arr[ri.second] = temp;
				return true;
			}
			return false;
		}

		int* arr;
	};

	struct node
	{
		wchar_t ch;
		int originalIndex;
		int index;
	};*/
	map<wchar_t, int> freqMap;
	deque<wchar_t> result;
	result.resize(inputText.size() - 1);
	//bwtOutput
	deque<wchar_t> bwtOutputSorted = inputText;
	sort(bwtOutputSorted.begin(), bwtOutputSorted.end());

	int* indices = new int[inputText.size()];
	int* oppositeIndex = new int[inputText.size()];
	for (int i = 0; i < inputText.size(); i++)
	{
		int charCount = ++freqMap[inputText[i]];
		indices[i] = i;
		int currentIndex = 
		oppositeIndex[i] = binarySearch(&bwtOutputSorted, 0, inputText.size(), inputText[i]);
		oppositeIndex[i] += charCount - 1;
	}
	//sort(bwtOutputSorted.begin(), bwtOutputSorted.end(), Local(indices));
	//for test
	vector<int> t(oppositeIndex, oppositeIndex + inputText.size());

	int currentIndex = 0;
	for (int i = inputText.size() - 2; i >= 0; i--)
	{
		result[i] = inputText[currentIndex];
		currentIndex = oppositeIndex[currentIndex];
	}
	//cout << "fgnbfgb" << endl;

	return result;
}

BWT::~BWT()
{
}
