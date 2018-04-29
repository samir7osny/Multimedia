#include "Huffman.h"

int main() {
	Huffman huffman;
	huffman.encode("DataSet_1.txt", "DataSet_1_compressed.bin");
	

	//Huffman huffman2;
	huffman.decode("DataSet_1_compressed.bin", "DataSet_1_decompressed.txt");

	system("pause");
}