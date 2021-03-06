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

#include <iostream>
#include <string>
#include <memory>
#include <tst_tree.hpp>
#include <tst_test.hpp>
#include <io_functions.h>
#include "my_vector.h"
#include <bitset>
#include <cassert>
#include "cmdline.h"
//#include "qgram_tree.h"

using namespace std;
using namespace my;

void count(tst::TST& tst, ifstream &pfs)
{
    string line;
    uint64_t n = 0;
    while (getline(pfs, line))
    {
        vector<uint64_t> occs;
        if (line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        std::cout << "query : " << ++n << " / " << line << std::endl;
        tst::istring pattern;
        my::createFromString(line, pattern);
        uint64_t result = tst.count(pattern);
        
        std::cout << "the pattern occurs " << result << " times" << std::endl;
        
    }

    pfs.close();
}

void locate(tst::TST& tst, ifstream &pfs)
{
    string line;
    uint64_t n = 0;
    while (getline(pfs, line))
    {
        uint64_t totalCount = 0;
        vector<uint64_t> occs;
        if (line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        std::cout << "query : " << ++n << " / " << line << std::endl;
        tst::istring pattern;
        my::createFromString(line, pattern);

        tst.locate(pattern, occs);
        if(occs.size() > 0){
            tst::istring resultString;

            for(auto& index : occs){
                tst::TST::TSTNodePointer pointer = tst.createLeafPointer(index);
                pointer.getPathString(resultString);
                
                if(tst.hasCountVec()){
                    //std::cout << my::to_string(resultString) << ", Count = " << tst.getCount(index) << std::endl;
                    uint64_t count = tst.getCount(make_pair(index, true));
                    std::cout << "q-gram : " << my::to_string(resultString) << ", Count : " << count << std::endl;
                    totalCount += count;
                }else{
                    std::cout << "q-gram : "  << my::to_string(resultString) << std::endl;
                }
                
            }

            if(tst.hasCountVec()){
                std::cout << "Total count : " << totalCount << std::endl;
            }
        }else{
            std::cout << "the pattern does not exist " << std::endl;
        }
    }

    pfs.close();
}


int main(int argc, char *argv[])
{

    cmdline::parser p;

    p.add<string>("input_file", 'i', "input file name", true);
    p.add<string>("query_file", 'q', "query file name", true);
    p.add<string>("mode", 'm', "choose 'locate' or 'count' query", false);

    p.parse_check(argc, argv);
    string file = p.get<string>("input_file");
    string query_file = p.get<string>("query_file");
    string mode = p.get<string>("mode");
    std::cout << "mode : " << mode << std::endl;
    tst::TST tree;
    ifstream inputStream;
    inputStream.open(file);
    tree.load(inputStream);
    inputStream.close();

    ifstream queryStream;
    queryStream.open(query_file);

    if(mode == "count"){

        if(tree.hasInternalCountVec()){
        count(tree, queryStream);
        }else{
            std::cout << "This TST does not have the count vector." << std::endl;
        }
    }else{
        locate(tree, queryStream);
    }

    std::cout << "Finish." << std::endl;
}
