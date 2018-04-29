#include "Huffman.h"



Huffman::Huffman(string fileName, string fileExtension) :fileName(fileName), fileExtension(fileExtension), file(fileName + "." + fileExtension, std::ios::binary)
{
	/*
		file->seekg(0, std::ios::end);
		inputText.reserve(file->tellg());
		file->seekg(0, std::ios::beg);

		inputText.assign((std::istreambuf_iterator<char>(*file)),
			std::istreambuf_iterator<char>());
		// // cout << inputText;*/
	averageCodeWord = 0;
	maxCodeLength = 0;
	numberOfChar = 0;
	numberOfNodes = 0;
}

void Huffman::encode(string originalFile, string compressedFile)
{
	clock_t Start = clock();

	if (originalFile != "")
	{
		file.close();
		file.open(originalFile , std::ios::binary);
	}
	if (compressedFile == "")
	{
		compressedFile = fileName + "_compressed.bin";
	}
	outFile.open(compressedFile, ios::out | ios::binary);
	outFile.clear();
	outBuffer = 0;
	outBufferCounter = 0;
	getText();
	buildTree();
	forward();
	reverse(Nodes[0]);
	// cout << numberOfChar << endl;
	// cout << codeCharMap.size() << endl;
	// cout << maxCodeLength << endl;
	// cout << numberOfNodes << endl;
	
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
				buffer((int)(j),3);
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

	file.close();
	outFile.close();

	cout << "Time to Compress: " << clock() - Start << endl;
}

void Huffman::decode(string CompressedFile, string DecompressedFile)
{
	clock_t Start = clock();
	
	if (CompressedFile == "")
	{
		CompressedFile = fileName + "_compressed.bin";
	}
	if (DecompressedFile == "")
	{
		DecompressedFile = fileName + "_decompressed.txt";
	}
	wifstream compressedFile(CompressedFile, ios::binary);
	wofstream decompressedFile(DecompressedFile, ios::binary);

	// apply BOM-sensitive UTF-16 facet
	decompressedFile.imbue(std::locale(decompressedFile.getloc(),
		new std::codecvt_utf8<wchar_t>));

	streampos fileSizePos = compressedFile.tellg();
	compressedFile.seekg(0, ios::end);
	fileSizePos = compressedFile.tellg() - fileSizePos;
	int fileSize = fileSizePos;
	compressedFile.close();
	compressedFile.open(CompressedFile , ios::binary);
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


	for (wchar_t c; compressedFile.get(c); ) {
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
									numberOfTakenBits++;
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
					if (decodeCounter!= 0 && counter == decodeCounter + 3)
					{
						skipBits = 8 - skipBits;
					}
				}
				else if (takenBytes != fileSize || (takenBytes == fileSize && skipBits < i + 1)) {
					HuffmanCode += (temp[i] ? "1" : "0");
					map<string, wchar_t>::iterator it = codeCharMap.find(HuffmanCode);
					if (it != codeCharMap.end()) {
						decompressedFile << it->second;
						HuffmanCode = "";
					}
					//// cout << temp[i];
				}
			}
		}
	}
	compressedFile.close();
	decompressedFile.close();

	cout << "Time to Decompress: " << clock() - Start << endl;
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

void Huffman::getText()
{
	// apply BOM-sensitive UTF-16 facet
	file.imbue(std::locale(file.getloc(),
		new std::codecvt_utf8<wchar_t>));
	// read 
	for (wchar_t c; file.get(c); ) {
		//std::// // cout << std::showbase << std::hex << c << '\n';
		if (c != 65279) {
			inputText.push_back(c);
			freqMap[c]++;
		}
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
		outFile << outBuffer;
		outBufferCounter = 0;
		outBuffer = 0;
	}
}



Huffman::~Huffman()
{
}
