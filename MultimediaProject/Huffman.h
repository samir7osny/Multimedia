#pragma once
#include "Headers.h"

struct Node {
	wchar_t character;
	double probabilty;
	string code;
	Node* left;
	Node* right;
};

class Huffman
{
private:
	deque<wchar_t> inputText;
	deque<unsigned char> Output;
	double averageCodeWord;
	double maxCodeLength;
	int numberOfChar;
	int numberOfNodes;

	unsigned char outBuffer;
	int outBufferCounter;

	map<wchar_t,int> freqMap;
	map<wchar_t, string> charCodeMap;
	vector<Node*> Nodes;

	void buildTree();
	void sortLastElement(vector<Node*>& v);
	void fillFreqMap();
	void findNodeByChar(Node*parent, wchar_t c,Node*&found);
	void forward();
	void reverse(Node* parent);
	void buffer(bool bit);
	void buffer(string bitsString);
	void buffer(unsigned char bits, int n);
	void buffer(int bits, int n);
	void closeBuffer();
	void excuteBuffer();
public:
	Huffman();
	deque<unsigned char> encode(deque<wchar_t> inputText);
	deque<wchar_t> decode(deque<unsigned char> inputText);
	~Huffman();
};

