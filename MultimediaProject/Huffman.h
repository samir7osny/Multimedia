#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <deque>
#include <map>
#include <locale>
#include <queue>
#include <codecvt>
#include <bitset>
#include <algorithm>
using namespace std;

struct Node {
	wchar_t character;
	double probabilty;
	string code;
	Node* left;
	Node* right;
};
//
//class CompareProb
//{
//public:
//	bool operator()(Node n1, Node n2) {
//		return n1.probabilty>n2.probabilty;
//	}
//};
//
//template <class T, class S, class C>
//S& Container(priority_queue<T, S, C>& q) {
//	struct HackedQueue : private priority_queue<T, S, C> {
//		static S& Container(priority_queue<T, S, C>& q) {
//			return q.*&HackedQueue::c;
//		}
//	};
//	return HackedQueue::Container(q);
//}

class Huffman
{
private:
	vector<wchar_t> inputText;
	string fileName;
	string fileExtension;
	wifstream file;
	ofstream outFile;
	double averageCodeWord;
	double maxCodeLength;
	int numberOfChar;
	int numberOfNodes;

	unsigned char outBuffer;
	int outBufferCounter;

	map<wchar_t,int> freqMap;
	map<wchar_t, string> charCodeMap;
	//priority_queue<Node, deque<Node>, CompareProb> Nodes;
	vector<Node*> Nodes;
	//vector<bool>encoded;

	void buildTree();
	void sortLastElement(vector<Node*>& v);
	void getText();
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
	Huffman(string fileName = "", string fileExtension = "txt");
	void encode(string originalFile = "", string compressedFile = "");
	void decode(string CompressedFile = "", string DecompressedFile = "");
	~Huffman();
};

