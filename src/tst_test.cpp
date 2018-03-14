/* 
 * Copyright (C) 2018, Takaaki Nishimoto, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */
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
#include <set>
#include "tst_test.hpp"
#include "tst_tree.hpp"

namespace tst{
		void TST::createTreeLog(vector<string>& log, int i) {
			/*
			log[i].clear();
			vector<string> r;
			r.clear();
			TSTNode* node = &this->leave[i];
			while (!node->isRoot())
			{
				r.push_back(this->truncatedText.substring(node->textPosition, node->edgeLength).to_string());
				node = &this->internalNodes[node->parent];
			}
			log[i] = "#";
			for (int x = r.size() - 1; x >= 0; x--)
			{
				log[i] += r[x];
				if(x>0)log[i] += "|";
			}
			*/
		}
	void TST::createTreeLog(vector<string>& log) {
			/*
			log.resize(this->leave.size());
			for (int i = 0; i < this->leave.size(); i++) {
				createTreeLog(log, i);
			}
			*/
		}
		void TST::getInfo() {
			/*
			vector<string> log;
			this->createTreeLog(log);

			auto rambda = [](const string& x, const string& y) -> int {
				return x.compare(y) == -1;
			};


			std::sort(log.begin(), log.end(), rambda);

			for (auto& it : log) {
				std::cout << it << std::endl;
			}
			for (int i = 1; i < log.size(); i++) {
				if (log[i] == log[i - 1]) throw "error";
				auto lcp = my::MyFunction::longestCommonPrefix(log[i], log[i - 1]);
				if (!(lcp == 1 || log[i][lcp - 1] == '|')) {
					throw "error";
				}
			}
			*/

		}
		void TST::getAllQgrams(vector<string> result) {
			/*
			result.resize(this->leave.size());
			auto rambda = [](const string& x, const string& y) -> int {
				return x.compare(y) == -1;
			};
			istring s;
			for (int i = 0; i < this->leave.size(); i++) {
				s.clear();
				this->getPathString(NodeIndex(i, true), s);
				result[i] = s.to_string();
			}

			std::sort(result.begin(), result.end(), rambda);
			*/
		}
		
		void TST::TSTNodePointer::getInfo() {
			/*
			istring s;
			this->getPathString(s);
			if (!this->isOnBottom() && !this->isOnRoot()) {

				auto node = index.second ? this->tree.leave[index.first] : this->tree.internalNodes[index.first];
				auto p = this->tree.truncatedText.substr(node.textPosition + this->position, node.edgeLength - this->position);
			std:cout << "pointer = " << s.to_string() << "*" << (*p).to_string();
			}
			else {
				std::cout << "pointer = " << s.to_string();
			}
			std::cout << ", pathLen = " << this->pathLength << " , pos = " << this->position;
			std::cout << std::endl;
			*/
		}
	void computeAllQgrams(string &text, int q, vector<string> result)
	{
		result.clear();

		set<string> r;
		for (int i = 0; i < (int)text.size() - q + 1; i++)
		{
			auto qgram = text.substr(i, q);
			r.insert(qgram);
		}
		for (auto& it : r) {
			result.push_back(it);
		}
		auto rambda = [](const string& x, const string& y) -> int {
			return x.compare(y) == -1;
		};


		std::sort(result.begin(), result.end(), rambda);

	}
	bool test(string& text, int q) {
		/*
		vector<string> naiveResult;
		vector<string> tstResult;

		computeAllQgrams(text, q, naiveResult);
		TST tst;
		istring itext(text);
		tst.construct(itext, q);
		tst.getAllQgrams(tstResult);

		if (naiveResult.size() != tstResult.size()) throw "error";
		for (int i = 0; i < naiveResult.size(); i++) {
			if (naiveResult[i] != tstResult[i]) throw "error";
		}
		*/
		return true;
	}
}