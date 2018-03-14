/*
* Copyright (c) 2018 Takaaki Nishimoto
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published bytes
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "tst_tree.hpp"
#include "my_vector.h"
#include <algorithm>
namespace tst
{
NodeIndex TST::ROOTINDEX = NodeIndex(0, false);
uint64_t TST::createRoot()
{
	this->createNode(TSTNode::EMPTY, 0, 0, false);
	this->internalNodes[0].parent = TSTNode::EMPTY;
	this->suffixLinks[0] = TSTNode::BOTTOM;
	return 0;
}
NodeIndex TST::createNode(uint64_t parent, uint64_t _textSourcePosition, uint64_t _edgeLength, bool isLeaf)
{
	if (isLeaf)
	{
		uint64_t id = this->leave.size();
		this->leave.push_back(TSTNode(parent, _textSourcePosition, _edgeLength));
		this->nextLeave.push_back(UINT64_MAX);
		//this->countNonLeaveVec.push_back(1);
		return NodeIndex(id, true);
	}
	else
	{
		uint64_t id = this->internalNodes.size();
		this->internalNodes.push_back(TSTNode(parent, _textSourcePosition, _edgeLength));
		this->suffixLinks.push_back(UINT64_MAX);
		this->childrens.push_back(vector<NodeIndex>());
		return NodeIndex(id, false);
	}
}
void TST::clear()
{
	this->truncatedLength = UINT64_MAX;
	this->internalNodes.clear();
	this->leave.clear();
	this->nextLeave.clear();
	this->suffixLinks.clear();
	this->truncatedText.clear();
	this->countNonLeaveVec.clear();
}
uint64_t TST::getPathLength(NodeIndex index)
{

	istring s;
	this->getPathString(index, s);
	return s.size();
}
TSTNode TST::getRoot()
{
	return this->internalNodes[0];
}
/*
	bool RPatriciaNode::IsRoot()
	{
	return this->parent == UINT64_MAX;
	}
	*/

InsertResult TST::getInsertIndex(NodeIndex index, ichar edgeCharacter)
{
	assert(index.first != UINT64_MAX - 1);
	//TSTNode& node = (*this)[index];
	if (index.second)
	{
		return make_pair(false, 0);
	}
	else if (this->childrens[index.first].size() == 0)
	{
		return make_pair(false, 0);
	}
	int start_pos = 0, end_pos = this->childrens[index.first].size() - 1;
	TSTNode &fstNode = (*this)[this->childrens[index.first][start_pos]];
	TSTNode &lstNode = (*this)[this->childrens[index.first][end_pos]];

	if (start_pos == end_pos)
	{
		auto diff = edgeCharacter - fstNode.getLabel(this->truncatedText);
		if (diff == 0)
		{
			return make_pair(true, 0);
		}
		else if (diff < 0)
		{
			return make_pair(false, 0);
		}
		else
		{
			return make_pair(false, 1);
		}
	}

	if (edgeCharacter <= fstNode.getLabel(this->truncatedText))
	{
		bool b = (edgeCharacter == fstNode.getLabel(this->truncatedText));
		return make_pair(b, 0);
	}
	else if (edgeCharacter >= lstNode.getLabel(this->truncatedText))
	{
		bool b = edgeCharacter == lstNode.getLabel(this->truncatedText);
		if (b)
		{
			return make_pair(true, end_pos);
		}
		else
		{
			return make_pair(false, end_pos + 1);
		}
	}
	else
	{
		while (end_pos - start_pos != 1)
		{
			int i = (start_pos + end_pos) / 2;
			auto &mNode = (*this)[this->childrens[index.first][i]];
			auto diff = mNode.getLabel(this->truncatedText) - edgeCharacter;
			if (diff == 0)
			{
				return make_pair(true, i);
			}
			else if (diff < 0)
			{
				start_pos = i;
			}
			else
			{
				end_pos = i;
			}
		}
		return make_pair(false, end_pos);
	}
}

void TST::getPathString(NodeIndex index, istring &result)
{
	result.clear();
	vector<istring> r;
	r.clear();
	NodeIndex now = index;
	TSTNode *node = &(*this)[index];
	if (index.first == TSTNode::BOTTOM)
		return;
	while (!node->isRoot())
	{
		vector<ichar> str;
		my::substr(this->truncatedText, node->textPosition, node->edgeLength, str);
		//string str = this->truncatedText.substring(node->textPosition, node->edgeLength).to_string();
		r.push_back(str);
		node = &this->internalNodes[node->parent];
	}
	for (int i = r.size() - 1; i >= 0; i--)
	{
		my::concat(result, r[i]);
		//result.push_back(r[i]);
	}
}
void TST::construct(istring &text, uint64_t qgram)
{
	this->clear();
	this->createRoot();
	this->truncatedLength = qgram;
	for (uint64_t i = 0; i < qgram - 1; i++)
	{
		this->truncatedText.push_back(text[i]);
	}
	assert(this->truncatedText.size() == qgram - 1);
	//std::cout << "text = " << text.to_string() << std::endl;

	TSTNodePointer pointer(*this, NodeIndex(0, false), 0);
	bool b = false;
	uint64_t i = 0;

	std::cout << std::endl;
	while (i < text.size())
	{
		auto newEdgeLength = pointer.getNextLeafEdgeLength();
		if (i + (newEdgeLength - 1) >= text.size())
			break;
		auto lastChar = text[i + (newEdgeLength - 1)];
		b = pointer.onlineConstruct(text[i], lastChar);
		if (b)
		{
			i++;
			if (i % (PROCESSBAR2) == 0)
			{
				std::cout << "\r processing : "
						  << "[" << (i / PROCESSBAR2) << "/" << (text.size() / PROCESSBAR2) << "]" << std::flush;
			}
		}
	}
	std::cout << std::endl;

	uint64_t lastIndex = this->nextLeave.size() - 1;
	if (this->nextLeave[lastIndex] == TSTNode::EMPTY)
	{
		TSTNodePointer pointer2(*this, NodeIndex(0, false), 0);
		istring lastString;
		this->getPathString(NodeIndex(lastIndex, true), lastString);
		bool hasNextSuffix = true;
		for (uint64_t x = 1; x < lastString.size(); x++)
		{
			bool b = pointer2.proceed(lastString[x]);
			if (!b)
			{
				hasNextSuffix = false;
				break;
			}
		}
		if (hasNextSuffix)
		{
			if (pointer2.index.second)
			{
				this->nextLeave[lastIndex] = pointer2.index.first;
			}
			else
			{
				this->nextLeave[lastIndex] = this->childrens[pointer2.index.first][0].first;
			}
		}
	}
}
void TST::save(ofstream &os)
{
	std::cout << "writing tst.." << std::endl;
	os.write((const char *)(&this->truncatedLength), sizeof(uint64_t));
	uint64_t internalNodeSize = this->internalNodes.size();
	uint64_t leaveSize = this->leave.size();
	uint64_t textSize = this->truncatedText.size();

	os.write((const char *)(&internalNodeSize), sizeof(uint64_t));
	os.write((const char *)(&leaveSize), sizeof(uint64_t));
	os.write((const char *)(&textSize), sizeof(uint64_t));

	os.write((const char *)(&this->internalNodes[0]), sizeof(TSTNode) * internalNodeSize);
	os.write((const char *)(&this->leave[0]), sizeof(TSTNode) * leaveSize);
	os.write((const char *)(&this->nextLeave[0]), sizeof(uint64_t) * leaveSize);
	os.write((const char *)(&this->truncatedText[0]), sizeof(ichar) * textSize);

	vector<ichar> eachChildrenSize(internalNodeSize, 0);
	for (uint64_t i = 0; i < internalNodeSize; i++)
	{
		eachChildrenSize[i] = this->childrens[i].size();
	}
	os.write((const char *)(&eachChildrenSize[0]), sizeof(ichar) * internalNodeSize);

	for (uint64_t i = 0; i < internalNodeSize; i++)
	{
		os.write((const char *)(&this->childrens[i][0]), sizeof(NodeIndex) * eachChildrenSize[i]);
	}
	bool createCountNonLeaveVec = this->countNonLeaveVec.size() > 0;
	os.write((const char *)(&createCountNonLeaveVec), sizeof(bool));
	if (createCountNonLeaveVec)
	{
		os.write((const char *)(&this->countNonLeaveVec[0]), sizeof(uint64_t) * this->countNonLeaveVec.size());
	}
	bool createCountLeaveVec = this->countLeaveVec.size() > 0;
	os.write((const char *)(&createCountLeaveVec), sizeof(bool));
	if (createCountLeaveVec)
	{
		os.write((const char *)(&this->countLeaveVec[0]), sizeof(uint64_t) * this->countLeaveVec.size());
	}

	std::cout << "finished." << std::endl;
}
void TST::load(ifstream &ifs)
{
	std::cout << "loading TST.." << std::endl;
	this->clear();
	ifs.read((char *)(&this->truncatedLength), sizeof(uint64_t));
	uint64_t internalNodeSize = 0;
	uint64_t leaveSize = 0;
	uint64_t textSize = 0;
	ifs.read((char *)(&internalNodeSize), sizeof(uint64_t));
	ifs.read((char *)(&leaveSize), sizeof(uint64_t));
	ifs.read((char *)(&textSize), sizeof(uint64_t));

	this->internalNodes.resize(internalNodeSize);
	ifs.read((char *)(&this->internalNodes[0]), sizeof(TSTNode) * internalNodeSize);
	std::cout << leaveSize << std::endl;
	this->leave.resize(leaveSize);
	ifs.read((char *)(&this->leave[0]), sizeof(TSTNode) * leaveSize);
	this->nextLeave.resize(leaveSize);
	ifs.read((char *)(&this->nextLeave[0]), sizeof(uint64_t) * leaveSize);
	this->truncatedText.resize(textSize);
	ifs.read((char *)(&this->truncatedText[0]), sizeof(ichar) * textSize);

	vector<ichar> eachChildrenSize(internalNodeSize, 0);
	ifs.read((char *)(&eachChildrenSize[0]), sizeof(ichar) * internalNodeSize);
	//uint64_t sum = 0;
	//for(uint i=0;i<internalNodeSize;i++){
	//	sum += eachChildrenSize[i];
	//}

	this->childrens.resize(internalNodeSize);

	for (uint i = 0; i < internalNodeSize; i++)
	{
		this->childrens[i].resize(eachChildrenSize[i]);
		ifs.read((char *)(&this->childrens[i][0]), sizeof(NodeIndex) * eachChildrenSize[i]);
	}
	if (this->childrens.size() == 0)
		throw "error";

	bool createdCountNonLeaveVec;
	ifs.read((char *)(&createdCountNonLeaveVec), sizeof(bool));
	if (createdCountNonLeaveVec)
	{
		this->countNonLeaveVec.resize(internalNodeSize);
		ifs.read((char *)(&this->countNonLeaveVec[0]), sizeof(uint64_t) * internalNodeSize);
	}
	else
	{
	}

	bool createdCountLeaveVec;
	ifs.read((char *)(&createdCountLeaveVec), sizeof(bool));
	if (createdCountLeaveVec)
	{
		this->countLeaveVec.resize(leaveSize);
		ifs.read((char *)(&this->countLeaveVec[0]), sizeof(uint64_t) * leaveSize);
	}
	else
	{
	}

	/*
	for (uint64_t i=0;i<this->leave.size();i++){

        tst::istring resultString;
        tst::TST::TSTNodePointer pointer = this->createLeafPointer(i);
        pointer.getPathString(resultString);
	    std::cout << my::to_string(resultString) << "/" << this->countLeaveVec[i] << ", Index = " << i << std::endl;
		
	}
	*/

	std::cout << "loaded." << std::endl;
}
void TST::getLeafLink(uint64_t leafIndex, vector<std::pair<uint64_t, ichar>> &result)
{
	result.clear();

	NodeIndex p(leafIndex, true);
	auto leafNode = (*this)[p];
	TSTNodePointer pointer(*this, p, leafNode.edgeLength);
	pointer.moveNextLeave();
	if (pointer.isOnChild())
	{
		for (auto it : this->childrens[pointer.index.first])
		{
			auto &nextLeafNode = (*this)[it];
			result.push_back(make_pair(it.first, this->truncatedText[nextLeafNode.textPosition]));
		}
	}
	else
	{
		auto &nextLeafNode = (*this)[pointer.index];
		result.push_back(make_pair(pointer.index.first, this->truncatedText[nextLeafNode.textPosition + pointer.position]));
	}
}
void TST::constructQgramTree(vector<vector<uint64_t>> &_childrens, vector<ichar> &_labels, vector<uint64_t> &qgramNodePositionsOfTSTLeave)
{
	_childrens.clear();
	_labels.clear();
	qgramNodePositionsOfTSTLeave.clear();

	vector<std::pair<uint64_t, ichar>> children;
	istring fstText;

	auto fstIndex = 0;
	this->getPathString(NodeIndex(0, true), fstText);
	assert(fstText.size() == this->truncatedLength);
	for (int i = 0; i <= this->truncatedLength; i++)
	{

		auto c = i > 0 ? fstText[i - 1] : 0;
		_labels.push_back(c);
		_childrens.push_back(vector<uint64_t>());
		if (i != this->truncatedLength)
		{
			_childrens[i].push_back(i + 1);
		}
	}

	qgramNodePositionsOfTSTLeave = vector<uint64_t>(this->leave.size(), UINT64_MAX);
	qgramNodePositionsOfTSTLeave[fstIndex] = _labels.size() - 1;
	std::stack<uint64_t> leafIndexStack;
	leafIndexStack.push(fstIndex);
	while (leafIndexStack.size() > 0)
	{
		auto leafIndex = leafIndexStack.top();
		leafIndexStack.pop();
		//leafIndex;
		if (this->nextLeave[leafIndex] == TSTNode::EMPTY)
			continue;
		this->getLeafLink(leafIndex, children);
		auto x = qgramNodePositionsOfTSTLeave[leafIndex];
		assert(x != UINT64_MAX);
		for (auto it : children)
		{
			if (qgramNodePositionsOfTSTLeave[it.first] == UINT64_MAX)
			{
				auto i = _labels.size();
				_labels.push_back(it.second);
				_childrens.push_back(vector<uint64_t>());
				_childrens[x].push_back(i);
				qgramNodePositionsOfTSTLeave[it.first] = i;
				leafIndexStack.push(it.first);
			}
		}
	}
	for (auto it : qgramNodePositionsOfTSTLeave)
	{
		if (it == UINT64_MAX)
			throw "error";
	}
}
uint64_t TST::getNextLeaf(uint64_t leafIndex, ichar label)
{
	NodeIndex p(leafIndex, true);
	auto leafNode = (*this)[p];
	TSTNodePointer pointer(*this, p, leafNode.edgeLength);
	pointer.moveNextLeave();
	bool b = pointer.proceed(label);
	if (b)
	{
		return pointer.index.first;
	}
	else
	{
		return UINT64_MAX;
	}
}
bool TST::suffixLinkCheck(uint64_t i)
{
	if (this->suffixLinks[i] != TSTNode::EMPTY && this->suffixLinks[i] != TSTNode::BOTTOM)
	{
		assert(this->getPathLength(NodeIndex(this->suffixLinks[i], false)) + 1 == this->getPathLength(NodeIndex(i, false)));
	}
	return true;
}

bool TST::translatePattern(istring &pattern, vector<uint64_t> &result, bool isText)
{
	result.clear();
	if (isText)
		result.resize(pattern.size() - this->truncatedLength + 1);

	TSTNodePointer pointer(*this, ROOTINDEX, 0);
	bool b = false;
	if ((pattern.size() > PROCESSBAR2))
		std::cout << std::endl;
	for (uint64_t i = 0; i < pattern.size(); i++)
	{
		if ((i + 1) % (PROCESSBAR2) == 0)
		{
			std::cout << "\r processing : "
					  << "[" << (i / PROCESSBAR2) << "/" << (pattern.size() / PROCESSBAR2) << "]" << std::flush;
		}
		b = pointer.proceed(pattern[i]);
		if (!b)
		{
			result.clear();
			return false;
		}
		if (pointer.isOnLeaf())
		{
			if (isText)
			{
				result[i - this->truncatedLength + 1] = pointer.index.first;
			}
			else
			{
				result.push_back(pointer.index.first);
			}
			if (i + 1 != pattern.size())
				pointer.moveNextLeave();
		}
	}
	if ((pattern.size() > PROCESSBAR2))
		std::cout << std::endl;
	return true;
}
bool TST::restoreText(vector<uint64_t> &text, istring &result, uint64_t start_pos, uint64_t end_pos){
	
	if(start_pos >= this->truncatedLength){
		throw -1;
	}
	uint64_t len = end_pos - start_pos + 1;
	result.clear();
	result.resize(len);
	vector<ichar> tmp;
	this->getPathString(NodeIndex(text[0], true), tmp);
	uint64_t min_pos = std::min((uint64_t)(tmp.size() - start_pos), len);
	uint64_t i;
	for(i = 0;i<min_pos;i++){
		result[i] = tmp[i+start_pos]; 
	}
	uint64_t p=1;
	while(i < len){
		result[i] = this->truncatedText[this->leave[text[p]].textPosition + this->leave[text[p]].edgeLength - 1];
		i++;
		p++;
	
	}
	return true;
}


bool TST::restoreText(vector<uint64_t> &text, istring &result)
{
	if(text.size() == 0) return true;
	uint64_t len = text.size() - 1 + this->truncatedLength;
	return this->restoreText(text, result, 0, len - 1);
	/*
	result.clear();
	//istring fstQgram;
	this->getPathString(NodeIndex(text[0], true), result);

	result.resize(text.size() + this->truncatedLength - 1);
	//for(uint64_t i =0;i<fstQgram.size();i++){
	//	result[i] = fstQgram[i];
	//}
	for (uint64_t i = 1; i < text.size(); i++)
	{
		result[i + this->truncatedLength - 1] = this->truncatedText[this->leave[text[i]].textPosition + this->leave[text[i]].edgeLength - 1];
	}
	return true;
	*/
}
void TST::locate(istring &pattern, vector<uint64_t> &result)
{
	result.clear();
	TSTNodePointer pointer(*this, ROOTINDEX, 0);
	bool b = false;
	for (auto it : pattern)
	{
		b = pointer.proceed(it);
		if (!b)
			return;
	}
	pointer.getLeave(result);
}
uint64_t TST::count(istring &pattern)
{
	TSTNodePointer pointer(*this, ROOTINDEX, 0);
	bool b = false;
	for (auto it : pattern)
	{
		b = pointer.proceed(it);
		if (!b)
		{
			return 0;
		}
	}
	uint64_t count = this->getCount(pointer.index);
	return count;
}
std::pair<bool, uint64_t> TST::countUsingInternalCountVec(istring &pattern)
{
	TSTNodePointer pointer(*this, ROOTINDEX, 0);
	bool b = false;
	for (auto it : pattern)
	{
		b = pointer.proceed(it);
		if (!b)
		{
			return make_pair(true, 0);
		}
	}
	if (pointer.index.second)
	{
		return make_pair(false, pointer.index.first);
	}
	else
	{
		return make_pair(true, this->countNonLeaveVec[pointer.index.first]);
	}
}

void TST::constructCountVec(const vector<uint64_t> &translatedText, bool clearCountLeaveVec)
{
	//vector<uint64_t> countLeaveVec(this->leave.size(), 0);
	//this->countLeaveVec(this->leave.size(), 0);
	this->countLeaveVec.resize(this->leave.size(), 0);
	for (auto it : translatedText)
	{
		this->countLeaveVec[it]++;
	}

	for (auto it : this->countLeaveVec)
		if (it == 0)
			throw "error";

	this->countNonLeaveVec.resize(this->internalNodes.size());
	for (uint64_t i = 0; i < this->countNonLeaveVec.size(); i++)
		this->countNonLeaveVec[i] = UINT64_MAX;

	auto checkChildrenCounted = [&](uint64_t node) {
		for (auto child : this->childrens[node])
		{
			if (!child.second && this->countNonLeaveVec[child.first] == UINT64_MAX)
			{
				return false;
			}
		}
		return true;
	};
	auto getCount = [&](uint64_t node) {
		uint64_t count = 0;
		for (auto child : this->childrens[node])
		{
			count += child.second ? countLeaveVec[child.first] : this->countNonLeaveVec[child.first];
		}
		return count;
	};

	stack<uint64_t> notCountedNonLeaveStack;
	notCountedNonLeaveStack.push(0);
	while (notCountedNonLeaveStack.size() > 0)
	{
		uint64_t top = notCountedNonLeaveStack.top();
		if (checkChildrenCounted(top))
		{
			this->countNonLeaveVec[top] = getCount(top);
			notCountedNonLeaveStack.pop();
		}
		else
		{
			for (auto child : this->childrens[top])
			{
				if (!child.second)
					notCountedNonLeaveStack.push(child.first);
			}
		}
	}

	if (clearCountLeaveVec)
	{
		this->countLeaveVec.resize(0);
	}
}
}
