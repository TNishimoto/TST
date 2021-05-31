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
#include <bitset>
#include <cassert>
#include "cmdline.h"
#include "src/common1/my_vector.h"
//#include "qgram_tree.h"

using namespace std;
using namespace my;

int main(int argc, char *argv[])
{

    cmdline::parser p;

    p.add<string>("input_file", 'i', "input file name", true);
    p.add<string>("output_file", 'o', "output file name", true);

    p.add<int>("qgram", 'q', "qgram length", true);
    p.add<int>("count", 'a', "append count vector?", false, 1);

    p.parse_check(argc, argv);
    string file = p.get<string>("input_file");
    string output_file = p.get<string>("output_file");

    int32_t qgram = p.get<int>("qgram");
    int32_t count = p.get<int>("count");

    //create tst
    string text = "";
    ifstream inputStream;
    inputStream.open(file);
    if (!inputStream)
    {
        std::cout << "No Input File!" << std::endl;
        return 1;
    }
    ofstream os;
    os.open(output_file);

    my::IO::load(inputStream, text);
    inputStream.close();
    //tst::istring itext(text);
    tst::istring itext;
    my::createFromString(text, itext);

    tst::TST tree;
    tree.construct(itext, qgram);

    if (count >= 1)
    {
        vector<uint64_t> vec;
        tree.translatePattern(itext, vec, true);
        tree.constructCountVec(vec, count == 2);
    }

    tree.printInfo();

    tree.save(os);
    os.close();

    std::cout << "Finish." << std::endl;
}
