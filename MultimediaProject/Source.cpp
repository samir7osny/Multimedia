#include "Huffman.h"
#include "BWT.h"
#include "MTF.h"
#include "RL.h"

deque<wchar_t> charList;
deque<wchar_t> inputText;

deque<wchar_t> getText(string fileName);


int main() {
	/*Huffman huffman;
	huffman.encode("DataSet_2.tsv", "DataSet_2_compressed.bin");
	

	Huffman huffman2;
	huffman2.decode("DataSet_2_compressed.bin", "DataSet_2_decompressed.txt");*/
	
	deque<wchar_t> inputText;
	inputText = getText("DataSet_1.txt");
	BWT bwt;
	MTF mtf;
	RL rle;
	Huffman huf;
	deque<wchar_t> bwtEncode = bwt.encode(inputText);
	deque<wchar_t> mtfEncode = mtf.encode(bwtEncode, charList, true);
	deque<wchar_t> rleEncode  = rle.encode(mtfEncode);
	deque<unsigned char> hufEncode = huf.encode(rleEncode);


	ofstream outFile("test3.bin", ios::out | ios::binary);
	outFile.clear();
	for (int i = 0; i < hufEncode.size(); i++)
	{
		outFile << hufEncode[i];
	}
	outFile.close();
	
	ifstream file("test3.bin", ios::binary);
	file.clear();
	deque<unsigned char> temp;
	for (char c; file.get(c); ) {
		temp.push_back(c);
	}
	file.close();

	deque<wchar_t> hufDecode = huf.decode(hufEncode);
	deque<wchar_t> rleDecode  = rle.decode(hufDecode);
	deque<wchar_t> mtfDecode = mtf.decode(rleDecode, charList, true);
	deque<wchar_t> bwtDecode = bwt.decode(mtfDecode);

	wofstream outFile2("test3_decompressed.txt", ios::out | ios::binary);
	outFile2.imbue(std::locale(outFile2.getloc(),
		new std::codecvt_utf8<wchar_t>));
	outFile2.clear();
	for (int i = 0; i < bwtDecode.size(); i++)
	{
		outFile2 << bwtDecode[i];
	}
	outFile2.close();
	/*deque<wchar_t> encoded = mtf.encode(bwt.encode(inputText),charList,true);
	deque<wchar_t> decoded = bwt.decode(mtf.decode(encoded,charList,true));*/
	system("pause");
}



deque<wchar_t> getText(string fileName)
{
	wifstream file(fileName, std::ios::binary);
	// apply BOM-sensitive UTF-16 facet
	file.imbue(std::locale(file.getloc(),
		new std::codecvt_utf8<wchar_t, 0xfff, std::consume_header>));
	// read     

	map<wchar_t,bool> charMap;
	for (wchar_t c; file.get(c); ) {
		//std::cout << std::showbase << std::hex << c << '\n';
		inputText.push_back(c);
		charMap[c];
	}
	for (map<wchar_t, bool>::iterator it = charMap.begin(); it != charMap.end(); it++)
	{
		charList.push_back(it->first);
	}
	return inputText;
}