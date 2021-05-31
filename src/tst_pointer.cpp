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
#include "tst_node.hpp"
#include "tst_tree.hpp"
#include <stack>

namespace tst
{
bool TST::TSTNodePointer::isOnBottom()
{
    return this->index.first == TSTNode::BOTTOM;
}
bool TST::TSTNodePointer::isOnRoot()
{
    return this->index.first == 0 && this->index.second == false;
}
bool TST::TSTNodePointer::isOnChild()
{
    if (this->isOnBottom())
    {
        return true;
    }
    else if (this->isOnRoot())
    {
        return true;
    }
    else
    {
        TSTNode node = index.second ? this->tree.leave[index.first] : this->tree.internalNodes[index.first];
        return node.edgeLength == this->position;
    }
}
bool TST::TSTNodePointer::isOnEdge()
{
    if (this->isOnRoot() || this->isOnBottom())
        return false;
    TSTNode node = index.second ? this->tree.leave[index.first] : this->tree.internalNodes[index.first];
    return node.edgeLength != this->position;
}
bool TST::TSTNodePointer::isOnLeaf()
{
    return this->isOnChild() && this->index.second == true;
}
TSTNode &TST::TSTNodePointer::node()
{
    return index.second ? this->tree.leave[index.first] : this->tree.internalNodes[index.first];
}
uint64_t TST::TSTNodePointer::getNextLeafEdgeLength()
{
    if (this->pathLength == (int)this->tree.truncatedLength)
    {
        return 1;
    }
    else
    {
        return this->tree.truncatedLength - this->pathLength;
    }
}
uint64_t TST::TSTNodePointer::getPathLength()
{
    istring s;
    this->getPathString(s);
    return s.size();
}
void TST::TSTNodePointer::moveNextLeave()
{
    if (!this->isOnLeaf())
        throw "error";
    if (this->tree.nextLeave[this->index.first] == TSTNode::EMPTY)
        throw "error";
    this->pathLength--;
    this->index = NodeIndex(this->tree.nextLeave[this->index.first], true);
    if (this->node().edgeLength == 1)
    {
        this->index = NodeIndex(this->node().parent, false);
        this->position = this->node().edgeLength;
    }
    else
    {
        this->position = this->node().edgeLength - 1;
    }
}
void TST::TSTNodePointer::getPathString(istring &result)
{
    result.clear();
    if (this->isOnBottom())
    {
        char c = '_';
        result.push_back((ichar)c);
    }
    else if (this->isOnRoot())
    {
    }
    else
    {
        TSTNode &node = index.second ? tree.leave[index.first] : tree.internalNodes[index.first];
        tree.getPathString(NodeIndex(node.parent, false), result);
        for (uint64_t x = 0; x < (uint64_t)this->position; x++)
        {
            result.push_back(tree.truncatedText[node.textPosition + x]);
        }
    }
}

bool TST::TSTNodePointer::proceed(ichar c)
{
    assert(this->index.first != TSTNode::EMPTY);
    if (this->isOnBottom())
    {
        this->index = NodeIndex(0, false);
        this->pathLength = 0;
        this->position = 0;
        return true;
    }

    //TSTNode& node = this->tree.getNode(index);
    if (this->isOnChild())
    {
        InsertResult childPos = this->tree.getInsertIndex(this->index, c);
        if (childPos.first)
        {
            this->index = this->tree.childrens[index.first][childPos.second];
            this->position = 1;
            this->pathLength++;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        ichar nowEdgeChar = tree.truncatedText[this->tree[index].textPosition + this->position];
        if (c == nowEdgeChar)
        {
            this->position++;
            this->pathLength++;
            return true;
        }
        else
        {
            return false;
        }
    }
}
void TST::TSTNodePointer::useSuffixLink()
{
    if (this->isOnRoot())
    {
        uint64_t bottom = TSTNode::BOTTOM;
        this->index = NodeIndex(bottom, false);
        //this->index = make_pair(TSTNode::BOTTOM, false);
        this->position = 0;
        this->pathLength = -1;
    }
    else
    {
        int32_t pos = this->position;
        NodeIndex preIndex = this->index;
        int32_t nextPathLength = this->pathLength - 1 - this->position;
        this->index = NodeIndex(tree.suffixLinks[this->tree[this->index].parent], false);

        this->position = this->isOnBottom() ? 0 : this->node().edgeLength;
        this->pathLength = nextPathLength;

        for (uint64_t i = 0; i < (uint64_t)pos; i++)
        {
            bool b = this->proceed(this->tree.truncatedText[this->tree[preIndex].textPosition + i]);
            if (!b)
                throw "error";
        }
        //this->getInfo();
    }
}
void TST::TSTNodePointer::split()
{
    assert(this->isOnEdge());
    if (this->index.second == false && this->index.first == 0)
        throw "error";

    if (this->position < 1 || this->position >= this->tree[this->index].edgeLength)
    {
        throw "error";
    }
    NodeIndex oldNodeIndex = this->index;
    ichar edgeChar = this->node().getLabel(this->tree.truncatedText);
    NodeIndex parentIndex(this->node().parent, false);

    InsertResult insertIndex = this->tree.getInsertIndex(parentIndex, edgeChar);
    assert(insertIndex.first);

    assert(this->node().parent != UINT64_MAX);

    NodeIndex newNodeIndex = this->tree.createNode(this->node().parent, this->node().textPosition, this->position, false);
    [[maybe_unused]]TSTNode &newNode = this->tree.internalNodes[newNodeIndex.first];
    this->tree.childrens[newNodeIndex.first].push_back(this->index);
    //newNode.children.push_back(this->index);

    this->tree[oldNodeIndex].textPosition += this->position;
    this->tree[oldNodeIndex].edgeLength -= this->position;

    [[maybe_unused]]TSTNode &oldParent = this->tree.internalNodes[this->tree[oldNodeIndex].parent];
    this->tree.childrens[this->tree[oldNodeIndex].parent][insertIndex.second] = newNodeIndex;
    //oldParent.children[insertIndex.second] = newNodeIndex;
    this->tree[oldNodeIndex].parent = newNodeIndex.first;

    this->index = newNodeIndex;
    assert((int)this->node().parent < (int)this->index.first);
    assert((int)this->pathLength == (int)this->getPathLength());
    if (!oldNodeIndex.second)
        assert(this->tree.suffixLinkCheck(oldNodeIndex.first));
    assert(this->tree.suffixLinkCheck(newNodeIndex.first));
    //std::cout << oldNodeIndex.first << oldNodeIndex.second << "/" << newNodeIndex.first << std::endl;
    //assert(this->tree.suffixLinkCheck());
}
uint64_t TST::TSTNodePointer::addLeaf(ichar newCharacter)
{
    if (!this->isOnChild())
        throw "error";
    this->tree.truncatedText.push_back(newCharacter);
    uint64_t newLeafEdgeLength = this->tree.truncatedLength - this->pathLength;
    uint64_t newLeafTextPosition = this->tree.truncatedText.size() - newLeafEdgeLength;
    assert(newLeafTextPosition + newLeafEdgeLength - 1 < this->tree.truncatedText.size());
    ichar fstChar = this->tree.truncatedText[newLeafTextPosition];
    InsertResult insertIndexResult = this->tree.getInsertIndex(index, fstChar);
    int insertIndex = insertIndexResult.second;
    if (insertIndexResult.first)
    {
        throw "error";
    }
    NodeIndex leafIndex = this->tree.createNode(this->index.first, newLeafTextPosition, newLeafEdgeLength, true);
    //this->node().children.insert(this->node().children.begin() + insertIndex, std::make_pair(leafIndex.first, true));
    this->tree.childrens[this->index.first].insert(this->tree.childrens[this->index.first].begin() + insertIndex, NodeIndex(leafIndex.first, true));

    if (this->tree.nextLeave.size() > 1 && this->tree.nextLeave[this->tree.nextLeave.size() - 2] == TSTNode::EMPTY)
    {
        this->tree.nextLeave[this->tree.nextLeave.size() - 2] = leafIndex.first;
    }

    return leafIndex.first;
}
bool TST::TSTNodePointer::onlineConstruct(ichar nowChar, ichar lastChar)
{

    if (this->isOnLeaf())
    {
        assert(this->pathLength == (int)this->tree.truncatedLength);
        if (this->tree.nextLeave[this->tree.nextLeave.size() - 1] == TSTNode::EMPTY)
        {
            this->tree.nextLeave[this->tree.nextLeave.size() - 1] = this->index.first;
        }
        this->moveNextLeave();
    }

    bool b = this->proceed(nowChar);
    if (!b)
    {
        [[maybe_unused]]NodeIndex nowChild = this->index;
        if (this->isOnEdge())
        {
            this->split();
            if (this->tree.suffixLinks[this->tree.internalNodes.size() - 2] == TSTNode::EMPTY)
            {
                this->tree.suffixLinks[this->tree.internalNodes.size() - 2] = this->index.first;
            }
            [[maybe_unused]]NodeIndex newInternalNodeIndex = this->index;

            this->addLeaf(lastChar);
            this->useSuffixLink();
            if (this->isOnChild() && this->tree.suffixLinks[this->tree.internalNodes.size() - 1] == TSTNode::EMPTY)
            {
                this->tree.suffixLinks[this->tree.internalNodes.size() - 1] = this->index.first;
            }
        }
        else
        {
            this->addLeaf(lastChar);
            this->useSuffixLink();
        }
        //this->truncatedText.push_back(text[i]);
    }
    return b;
}

void TST::TSTNodePointer::getLeave(vector<uint64_t> &result)
{
    std::stack<NodeIndex> stack;
    result.clear();
    //stack.clear();
    stack.push(this->index);
    while (stack.size() > 0)
    {
        NodeIndex top = stack.top();
        stack.pop();
        if (top.second)
        {
            result.push_back(top.first);
        }
        else
        {
            for (auto it : this->tree.childrens[top.first])
            {
                stack.push(it);
            }
        }
    }
}
bool TST::TSTNodePointer::moveToNextLeaf(ichar label)
{
    if (!this->index.second)
        throw "error";
    this->moveNextLeave();
    bool b = this->proceed(label);
    return b;
}
}
