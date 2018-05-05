#include "Huffman.h"



Huffman::Huffman() 
{
	averageCodeWord = 0;
	maxCodeLength = 0;
	numberOfChar = 0;
	numberOfNodes = 0;
}

deque<unsigned char> Huffman::encode(deque<wchar_t> inputText)
{
	clock_t Start = clock();
	outBuffer = 0;
	outBufferCounter = 0;
	this->inputText = inputText;
	fillFreqMap();
	buildTree();
	forward();
	reverse(Nodes[0]);
	
	// out number of bits required to store the number of characters
	unsigned char numberOfBitsForNumberOfChars = ceil(log2(numberOfChar) + 1);
	buffer((unsigned char)(numberOfBitsForNumberOfChars - 1), 4);

	// number of charcters
	buffer((int)numberOfChar, numberOfBitsForNumberOfChars);

	// out number of max bits required to store the number of characters
	unsigned char numberOfBitsOfMaxNumberOfCodeBits = ceil(log2(maxCodeLength) + 1);
	buffer((unsigned char)(numberOfBitsOfMaxNumberOfCodeBits), numberOfBitsForNumberOfChars);

	// out char code and its huffman code
	for (map<wchar_t, string>::iterator it = charCodeMap.begin(); it != charCodeMap.end(); ++it)
	{
		// 15 14 13 12 11 10 9 8 || 7 6 5 4 3 2 1 0
		// 0  1  2  3  4  5  6 7 || 7 6 5 4 3 2 1 X
																								// wchar_t tempMask = 65535;
																								// tempMask = tempMask & it->first;
		bitset<16> tempMaskSet(it->first);
		int j;
		for (j = 15; j >= 0; j--)
		{
			if (tempMaskSet[j] == 1)
				break;
		}
		if (j >= 8 && j != 15)	// To Ignore Technique
		{
			buffer(0); // For Ignore
			buffer((int)(15 - j), 3); // Number of the bits will be ignored
			for (int k = j; k >= 0; k--)
			{
				buffer(tempMaskSet[k]);
			}
		} 
		else { // To Take Technique
			if (j == 15) // Take all 16 bits :(
			{
				buffer(1);
				buffer((int)(0), 3); // Flag to git 16 bits
				buffer((wchar_t)(it->first), 16);
			}
			else
			{
				buffer(1);
				buffer((int)(j + 1),3);
				for (int k = j; k >= 0; k--)
				{
					buffer(tempMaskSet[k]);
				}
			}
		}

		// Write the huffman code
		buffer((int)(it->second.size() - 1), numberOfBitsOfMaxNumberOfCodeBits);
		buffer(it->second);
	}

	int bitsCounter = 3;	// First three bits to know the number of the bits will be taken in the last byte
	for (int i = 0; i < inputText.size(); i++)
	{
		bitsCounter += charCodeMap[inputText[i]].size();
	}
	int lastByte = bitsCounter % 8;
	// 0 1 2 3 4 5 6 7 
	// 0 1 2 3 4 5 6 7

	// Fix bits offset
	lastByte = (lastByte - (8 - outBufferCounter)) < 0 ? (8 - ((8 - outBufferCounter) - lastByte)) : (lastByte - (8 - outBufferCounter));				// number of the bits will be taken in the last byte
	buffer((int)lastByte, 3);
	for (int i = 0; i < inputText.size(); i++)
	{
		buffer(charCodeMap[inputText[i]]);
	}
	closeBuffer();

	cout << "Time to Compress: " << clock() - Start << endl;
	return Output;
}

deque<wchar_t> Huffman::decode(deque<unsigned char> inputText)
{
	clock_t Start = clock();
	
	deque<wchar_t> DOutput;
	int fileSize = inputText.size();
	int takenBytes = 0;
	int counter = 0;
	unsigned char numberOfBitsForNumberOfChars = 0;
	unsigned char numberOfBitsOfMaxNumberOfCodeBits = 0;
	bool inChar = false;
	int charBeginCounter;
	bool ignore = false;
	int numberOfTakenBits;
	int numberOfCodeBits;
	wchar_t charUTF;
	string code;
	int charCounter = 0;
	int decodeCounter = 0;
	int skipBits = 0;
	string HuffmanCode = "";
	map<string, wchar_t> codeCharMap;
	numberOfChar = 0;


	for (int j = 0; j < fileSize;j++) {
		wchar_t c = inputText[j];
		takenBytes++;
		bitset<8> temp(c);
		for (int i = 7; i >= 0; i--)
		{
			counter++;
			if (counter <= 4)
			{
				numberOfBitsForNumberOfChars = (numberOfBitsForNumberOfChars << 1) | (temp[i]);
			}
			else if (counter <= 4 + numberOfBitsForNumberOfChars + 1)
			{
				numberOfChar = (numberOfChar << 1) | (temp[i]);
			}
			else if (counter <= 4 + (numberOfBitsForNumberOfChars + 1) * 2)
			{
				numberOfBitsOfMaxNumberOfCodeBits = (numberOfBitsOfMaxNumberOfCodeBits << 1) | (temp[i]);
			}
			else if(charCounter != numberOfChar)
			{
				if (!inChar)
				{
					inChar = true;
					charBeginCounter = counter;
					if (temp[i])
						ignore = false;
					else
						ignore = true;
					numberOfTakenBits = 0;
					numberOfCodeBits = 0;
					charUTF = 0;
					code = "";
				}
				else
				{
					if (counter <= charBeginCounter + 3)
					{
						numberOfTakenBits = (numberOfTakenBits << 1) | (temp[i]);
						if (counter == charBeginCounter + 3)
						{
							if (ignore)
							{
								numberOfTakenBits = 16 - numberOfTakenBits;
							}
							else
							{
								if (numberOfTakenBits == 0)
								{
									numberOfTakenBits = 16;
								}
								else
								{
									numberOfTakenBits;
								}
							}
						}
					}
					else if (counter <= charBeginCounter + 3 + numberOfTakenBits) {
						charUTF = (charUTF << 1) | (temp[i]);
					}
					else if (counter <= charBeginCounter + 3 + numberOfTakenBits + numberOfBitsOfMaxNumberOfCodeBits) {
						numberOfCodeBits = (numberOfCodeBits << 1) | (temp[i]);
						if (counter == charBeginCounter + 3 + numberOfTakenBits + numberOfBitsOfMaxNumberOfCodeBits)
						{
							numberOfCodeBits++;
						}
					}
					else if (counter <= charBeginCounter + 3 + numberOfTakenBits + numberOfBitsOfMaxNumberOfCodeBits + numberOfCodeBits) {
						if (temp[i])
						{
							code += "1";
						}
						else
						{
							code += "0";
						}
						if (counter == charBeginCounter + 3 + numberOfTakenBits + numberOfBitsOfMaxNumberOfCodeBits + numberOfCodeBits)
						{
							codeCharMap[code] = charUTF;
							charCounter++;
							inChar = false;
						}
					}
				}
			}
			else
			{
				if (decodeCounter == 0 || counter <= decodeCounter + 3) {
					decodeCounter = decodeCounter == 0 ? counter - 1 : decodeCounter;
					skipBits = (skipBits << 1) | (temp[i]);
					/*if (decodeCounter!= 0 && counter == decodeCounter + 3)
					{
						skipBits = 8 - skipBits;
					}*/
				}
				else if (takenBytes != fileSize || (takenBytes == fileSize && skipBits != i + 1)) {
					HuffmanCode += (temp[i] ? "1" : "0");
					map<string, wchar_t>::iterator it = codeCharMap.find(HuffmanCode);
					if (it != codeCharMap.end()) {
						DOutput.push_back(it->second);
						HuffmanCode = "";
					}
					//// cout << temp[i];
				}
			}
		}
	}

	cout << "Time to Decompress: " << clock() - Start << endl;
	return DOutput;
}

void Huffman::buildTree()
{
	for (map<wchar_t, int>::iterator it = freqMap.begin(); it != freqMap.end(); it++)
	{
		Node* node = new Node;
		node->left = node->right = NULL;
		node->character = it->first;
		node->probabilty = (float)it->second / (float)inputText.size();
		Nodes.push_back(node);
		sortLastElement(Nodes);
	}
}

void Huffman::sortLastElement(vector<Node*>& v)
{
	for (vector<Node*>::reverse_iterator it = v.rbegin(), it2 = v.rbegin() + 1; it2 != v.rend(); it++, it2++)
	{
		if ((*it)->probabilty > (*it2)->probabilty) {
			Node* temp = *it;
			*it = *it2;
			*it2 = temp;
		}
		else {
			break;
		}
	}
}

void Huffman::fillFreqMap()
{
	for (int i = 0; i < inputText.size(); i++)
	{
		freqMap[inputText[i]]++;
	}
}

void Huffman::findNodeByChar(Node*parent, wchar_t c,Node*&found)
{
	if (parent->left == NULL && parent->right == NULL&&parent->character == c && !found)
	{
		found = parent;
	}

	if (parent->right != NULL&&!found) 
		findNodeByChar(parent->right, c, found);
	
	if (parent->left != NULL&&!found) 
		findNodeByChar(parent->left, c, found);
}

void Huffman::forward()
{
	while (Nodes.size() > 1) {
		Node* newNode = new Node;
		newNode->left = Nodes[Nodes.size() - 1];
		Nodes.pop_back();
		newNode->right = Nodes[Nodes.size() - 1];
		Nodes.pop_back();
		newNode->probabilty = newNode->left->probabilty + newNode->right->probabilty;
		// // cout << newNode->probabilty << endl;
		Nodes.push_back(newNode);
		sortLastElement(Nodes);
	}
}

void Huffman::reverse(Node* parent/*,vector<bool>code*/)
{
	numberOfNodes++;
	if (parent->right != NULL) {
		parent->right->code = parent->code;
		parent->right->code += ("1");
		reverse(parent->right);
	}
	if (parent->left != NULL) {
		parent->left->code = parent->code;
		parent->left->code += ("0");
		reverse(parent->left);
	}
	if (parent->left == NULL&&parent->right == NULL) {
		parent->code = parent->code == "" ? "0" : parent->code;
		numberOfChar++;
		// cout << "char:" << parent->character << "   binary:" << bitset<16>(parent->character) << "  code:" << parent->code;
		averageCodeWord += parent->code.size() * parent->probabilty;
		// cout << endl;
		if (parent->code.size() > maxCodeLength)
		{
			maxCodeLength = parent->code.size();
		}
		charCodeMap[parent->character] = parent->code;
		//codeCharMap[parent->code] = parent->character;
		return;
	}
}

void Huffman::buffer(bool bit)
{
	outBuffer = (outBuffer << 1) | (bit ? 1 : 0);
	outBufferCounter++;
	excuteBuffer();
}

void Huffman::buffer(string bitsString)
{
	for (int i = 0; i < bitsString.size(); i++)
	{
		buffer(bitsString[i] - 48);
	}
}

void Huffman::buffer(unsigned char bits, int n)
{
	unsigned char mask = 128;
	mask = mask >> (8 - n);
	for (int i = 0; i < n; i++)
	{
		buffer(mask & bits);
		mask = mask >> 1;
	}
}

void Huffman::buffer(int bits, int n)
{
	int mask = 32768;
	mask = mask >> (16 - n);
	for (int i = 0; i < n; i++)
	{
		buffer(mask & bits);
		mask = mask >> 1;
	}
}

void Huffman::closeBuffer()
{
	if (outBufferCounter)
	{
		int n = 8 - outBufferCounter;
		for (int i = 0; i < n; i++)
		{
			buffer(0);
		}
	}
}

void Huffman::excuteBuffer()
{
	if (outBufferCounter == 8)
	{
		Output.push_back(outBuffer);
		outBufferCounter = 0;
		outBuffer = 0;
	}
}



Huffman::~Huffman()
{
}
