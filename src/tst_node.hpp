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
//#include "my_function.h"
//#include "zstring.h"
#include <cassert>
using namespace std;
namespace tst{
    using istring = vector<uint8_t>;
	using ichar = uint8_t;
	using ochar = uint64_t;
	using NodeIndex = std::pair<uint64_t, bool>;
	using InsertResult = std::pair<bool, uint64_t>;

	struct TSTNode
	{

	public:

		uint64_t parent;
		uint64_t textPosition;
		uint16_t edgeLength;
		//std::vector<std::pair<uint64_t, bool>> children;

		static const uint64_t BOTTOM = UINT64_MAX - 1;
		static const uint64_t EMPTY = UINT64_MAX;

		TSTNode() : parent(EMPTY), textPosition(UINT64_MAX), edgeLength(UINT16_MAX)
		{
			//children.clear();
		}
		TSTNode(uint64_t _parent, uint64_t _textPos, uint64_t _textLength) : parent(_parent), textPosition(_textPos), edgeLength(_textLength)
		{
			//children.clear();			
		}
		ichar getLabel(istring& text) {
			return text[this->textPosition];
		}
		bool isRoot(){
			return this->parent == UINT64_MAX;
		}
		
		
	};

}