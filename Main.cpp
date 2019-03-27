//============================================================================
// Name        : Main.cpp
// Author      : Brock Matzenbacher
// Version     :
// Copyright   : Pfft.
// Description : Runs the BitTree
//============================================================================

#include <iostream>
#include "sha256.h"
#include "BitTree.h"


unsigned int hexCharToVal(char c) {
	return (c >= 'a') ? (c - 'a' + 10) : (c - '0');
}

int main() {
	//a string to generate a hash of
	std::string s = "To many this answer is less appealing than the question.";
	//the hashing function returns this
	//as a 64 character hex string
	std::string hexStr = sha256(s);

	//where the magic happens
	BitTree myTree;

	//the base bit index
	unsigned int idx = 0;
//parse every character
	for (unsigned int i = 0; i < hexStr.length();i++) {
		unsigned int val = hexCharToVal(hexStr.at(i));

		//each character represents 4 bits (0-f)
		for (int j=0;j<4;j++) {
			//if the bit we are examining is set
			if ((val >> j) & 1) {
				myTree.set(j+idx);//add to bittree
			}
		}
		//increment the base bit index
		idx+=4;
	}
	//this will output the tree
	myTree.visualise();
	std::string str = sha256(s);
	std::cout << "   ";
	for (int i = 0; i < 64;i++) {
		std::cout << str[i] << "   |   ";
	}
	std::cout << std::endl;
	unsigned int full,left,right,leaf;
	std::cout <<"Statistics:" <<std::endl
			<< "Full Nodes: " <<(full=myTree.countFullNodes())
			<< "\nLeft Nodes: " << (left=myTree.countLeftNodes())
			<< "\nRight Nodes: "<< (right=myTree.countRightNodes())
			<< "\nLeaf Nodes: " << (leaf=myTree.countLeafNodes());
	std::cout << std::endl << myTree.compress();
	return 0;

}
