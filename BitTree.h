/*
 * BitTree.h
 *
 *  Created on: Oct 24, 2017
 *      Author: brock
 */

#ifndef BITTREE_H_
#define BITTREE_H_
#include<bits/stdc++.h>
#include <bitset>
#include <stddef.h>
#include <iostream>

#include <stack>
class BitTree {
private:
	std::bitset<512> bittree;
	//get index of parent
	unsigned int getParentIdx(unsigned int idx) {
		if (idx == 0) {
			return 0;
		}
		return ((idx-1) >> 1);
	}
	//get value of parent
	bool getParent(unsigned int idx) {
		return bittree[getParentIdx(idx)];
	}
	//get index of left child
	unsigned int getLeftChildIdx(unsigned int idx) {
		return (idx<<1)+1;
	}
	//get index of right child
	unsigned int getRightChildIdx(unsigned int idx) {
		return (idx+1) << 1;
	}
	//get value of left child
	bool getLeftChild(unsigned int idx) {
		if (idx >= 255) {
			return false;
		}
		return bittree[getLeftChildIdx(idx)];
	}
	//get value of right child
	bool getRightChild(unsigned int idx) {
		if (idx >= 255) {
			return false;
		}
		return bittree[getRightChildIdx(idx)];
	}
public:


	int set(unsigned int idx) {
		int count = 0;
		unsigned int valIdx = idx+255;

		if (bittree[valIdx])
			return count;
		while (valIdx > 0) {

			if (!bittree[valIdx]) {
				++count;
				bittree[valIdx]=true;
			}
			valIdx = getParentIdx(valIdx);
		}
		if (!bittree[valIdx]) {
			++count;
			bittree[valIdx]=true;
		}
		//std::cout << bittree.to_string() << std::endl;
		return count;
	}
	BitTree() {
	}
	virtual ~BitTree() {

	}
	//counts # of nodes w/ 2 children
	unsigned int countFullNodes() {
		unsigned int count = 0;
		std::stack<unsigned int> stack;
		stack.push(0);
		unsigned int idx;
		while (!stack.empty()) {
			idx = stack.top();
			stack.pop();
			bool left = getLeftChild(idx);
			bool right= getRightChild(idx);
			if (left)
				stack.push(getLeftChildIdx(idx));
			if (right)
				stack.push(getRightChildIdx(idx));
			if (left&&right)
				++count;
		}
		return count;
	}
	//# nodes w/ no children
	unsigned int countLeftNodes() {
		unsigned int count = 0;
		std::stack<unsigned int> stack;
		stack.push(0);
		unsigned int idx;
		while (!stack.empty()) {
			idx = stack.top();
			stack.pop();
			bool left = getLeftChild(idx);
			bool right= getRightChild(idx);
			if (left)
				stack.push(getLeftChildIdx(idx));
			if (right)
				stack.push(getRightChildIdx(idx));
			if (left&&!right)
				++count;
		}
		return count;
	}
	//# of right only children
	unsigned int countRightNodes() {
		unsigned int count = 0;
		std::stack<unsigned int> stack;
		stack.push(0);
		unsigned int idx;
		while (!stack.empty()) {
			idx = stack.top();
			stack.pop();
			bool left = getLeftChild(idx);
			bool right= getRightChild(idx);
			if (left)
				stack.push(getLeftChildIdx(idx));
			if (right)
				stack.push(getRightChildIdx(idx));
			if (!left&&right)
				++count;
		}
		return count;
	}
//# left only children
	unsigned int countLeafNodes() {
		unsigned int count = 0;
		std::stack<unsigned int> stack;
		stack.push(0);
		unsigned int idx;
		while (!stack.empty()) {
			idx = stack.top();
			stack.pop();
			bool left = getLeftChild(idx);
			bool right= getRightChild(idx);
			if (left)
				stack.push(getLeftChildIdx(idx));
			if (right)
				stack.push(getRightChildIdx(idx));
			if (!left&&!right)
				++count;
		}
		return count;
	}
	//converts to a binary string of the bittree
	std::string compress() {

		unsigned int freq[4];


		freq[0]=countLeafNodes();
		freq[1]=countLeftNodes();
		freq[2]=countRightNodes();
		freq[3]=countFullNodes();
		unsigned int sum=0;
		for (int i = 0;i<4;i++){
			sum+=freq[i];
		}
		//total length of new dataset
		sum<<=1;
		const size_t DATA_SIZE = 1024;
		std::bitset<DATA_SIZE> dataSet;
		std::stack<char> stack;
		stack.push(0);
		char idx;
		int writeIdx = 0;
		while (!stack.empty()) {

			idx = stack.top();
			stack.pop();
			bool left = getLeftChild(idx);
			bool right= getRightChild(idx);
			unsigned int leftIdx=getLeftChildIdx(idx);
			unsigned int rightIdx=getRightChildIdx(idx);
			if (left) {
				stack.push(leftIdx);
				dataSet[DATA_SIZE-writeIdx++] = true;
			} else {
				writeIdx++;
			}
			if (right) {
				stack.push(rightIdx);
				dataSet[writeIdx++] = true;

			} else {
				++writeIdx;
			}

		}
		std::string bitString = dataSet.to_string('0','1').substr(DATA_SIZE-writeIdx,writeIdx);
		//std::reverse(bitString.begin(),bitString.end());
		return bitString;
	}
	void visualise() {


		const char LINE = '-';
		const char CARET = '^';
		const char SPACE = ' ';
		//start with last row(value)


		std::string line="";
		std::string nextLine = "";
		for (int i = 0; line.length() < 512;i++) {
			line+="V^V ";
		}
		std::string list[9];
		for (int i = 0; i <= 8;i++) {
			unsigned int MAX = 0x80 >> i;


			int count = 0;
			int swaps = 0;
			bool spaceMode = true;
			unsigned int varCount = 0;
			for (unsigned int j = 0; j < line.length();j++) {
				//std::cout <<i << " "<<varCount << " " << MAX << std::endl;
				if (line[j] == CARET) {
					nextLine+="V";
					if (++varCount >= MAX)
						break;
					spaceMode=!spaceMode;

					++swaps;
					count = 0;
					//std::cout << valueIdx << " " << caretIdx << " " << diff << "\n";
					continue;
				} else if (line[j] == 'V') {

					if (spaceMode)
						nextLine+=SPACE;
					else {

						nextLine+="-";
						count++;
					}
					//std::cout << valueIdx << " " << caretIdx << " " << diff << "\n";
				} else if (line[j]==SPACE) {
					if (spaceMode)
						nextLine+=SPACE;

					else {
						if (count > 0) {
							if (--count ==0 ) {
								nextLine+=CARET;
							} else {
								nextLine+=LINE;
							}
						} else {
							nextLine+=LINE;
						}

					}
				} else {
					if (spaceMode)
						nextLine+=SPACE;
					else {
						nextLine+="-";
						count++;
					}
				}


			}

			list[i]=line;
			line = nextLine;
			nextLine="";

		}
		int idx = 0;


		for (int i = 8;i>=0;i--) {
			size_t charIdx;

			while ((charIdx= list[i].find_first_of('V'))!=std::string::npos) {
				//std::cout << (bittree[idx] ? '@':'O');
				//std::cout << i << " "<<charIdx << " " << list[i].length() <<" "<<(bittree[idx] ?"@":"O")<< std::endl;
				if (i <= 8 && charIdx < list[i].length())
					list[i].replace(charIdx,1,bittree[idx] ?"@":"O");
				idx++;
			}
		}

		for (int i = 8;i>=0;i--) {

			std::cout << list[i] << std::endl;
		}

	}

};
#endif /* BITTREE_H_ */
