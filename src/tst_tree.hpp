#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include <algorithm>
#include <stdint.h>
#include <cassert>
//#include "my_function.h"
//#include "string_functions.h"
//#include "zstring.h"
#include "tst_node.hpp"
#include <stack>

namespace tst
{
//class TSTNodePointer;

class TST
{
  public:
  class TSTNodePointer
	{
	  public:
		TST &tree;
		NodeIndex index;
		int32_t position;
		int32_t pathLength;
		bool isOnBottom();
		bool isOnRoot();
		bool isOnEdge();
		bool isOnLeaf();
		TSTNode &node();
		void getPathString(istring &result);
		uint64_t getPathLength();
		bool proceed(ichar c);
		void useSuffixLink();
		uint64_t addLeaf(ichar newCharacter);
		void split();

		bool isOnChild();
		void moveNextLeave();
		void getLeave(vector<uint64_t>& result);
		void getInfo();
		bool onlineConstruct(ichar nowChar, ichar lastChar);
		uint64_t getNextLeafEdgeLength();
		bool moveToNextLeaf(ichar label);
		TSTNodePointer(TST &_tree, NodeIndex _index, int _pos) : tree(_tree), index(_index), position(_pos), pathLength(0)
		{
		}
		
	};

  private:
	void createTreeLog(vector<string> &log, int i);
	void createTreeLog(vector<string> &log);
	void getInfo();
	bool suffixLinkCheck(uint64_t i);
	uint64_t createRoot();
	NodeIndex createNode(uint64_t parent, uint64_t _textSourcePosition, uint64_t _edgeLength, bool isLeaf);
	InsertResult getInsertIndex(NodeIndex index, ichar edgeCharacter);
	TSTNode &operator[](NodeIndex index) { return index.second ? this->leave[index.first] : this->internalNodes[index.first]; }
	void getPathString(NodeIndex index, istring &result);
	uint64_t getPathLength(NodeIndex index);

	vector<TSTNode> internalNodes{};
	vector<TSTNode> leave{};
	vector<uint64_t> nextLeave{};
	vector<uint64_t> suffixLinks{};
	vector<uint64_t> countNonLeaveVec{};
	vector<uint64_t> countLeaveVec{};

	vector<vector<NodeIndex>> childrens{};
	istring truncatedText;

    static const uint64_t PROCESSBAR = 10000;
    static const uint64_t PROCESSBAR2 = 1000000;
	static NodeIndex ROOTINDEX;
  public:

	uint64_t truncatedLength;

	void construct(istring &text, uint64_t qgram);
	void constructCountNonLeaveVec(const vector<uint64_t>& translatedText);
	TSTNode getRoot();
	void clear();
	void getAllQgrams(vector<string> result);
	void save(ofstream &os);
	void load(ifstream &ifs);
	void getLeafLink(uint64_t leafIndex, vector<std::pair<uint64_t, ichar>>& result);
	uint64_t getNextLeaf(uint64_t leafIndex, ichar label);

	TST() : truncatedLength(0)
	{
	}
	void constructQgramTree(vector<vector<uint64_t>>& _childrens, vector<ichar>& _labels, vector<uint64_t>& qgramNodePositionsOfTSTLeave);
	void locate(istring& pattern, vector<uint64_t>& result);
	uint64_t count(istring& pattern);
	bool translatePattern(istring& pattern, vector<uint64_t>& result, bool isText);
	bool restoreText(vector<uint64_t>& text, istring& result);
	bool hasInternalCountVec(){
		return this->countNonLeaveVec.size() == this->internalNodes.size();
	}
	bool hasCountVec(){
		return this->countNonLeaveVec.size() == this->internalNodes.size() && this->countLeaveVec.size() == this->leave.size();
	}
	uint64_t getCount(NodeIndex index){
		//std::cout << leafIndex << "/" << this->countNonLeaveVec.size() << std::endl;
		if(index.second){
			return this->countLeaveVec[index.first];			
		}else{
			return this->countNonLeaveVec[index.first];						
		}
	}
	TSTNodePointer createLeafPointer(uint64_t leafIndex){
		NodeIndex p(leafIndex, true);
		TSTNode leaf = (*this)[p];
		TSTNodePointer pointer(*this, p, leaf.edgeLength);
		pointer.pathLength = this->truncatedLength;
		return pointer;
	}
	void print(){
		//std::cout << tree.countNonLeaveVec.size() << std::endl;
        for(auto& p : this->countNonLeaveVec){
            std::cout << p << ", ";
        }
	}
	/*
	void getPathStringFromLeafIndex(uint64_t index, istring& result){
			TSTNodePointer pointer = this->createLeafPointer(index);
			return pointer.getPathString(result);
	}
	*/
};
}