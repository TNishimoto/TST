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
#include <memory>
#include <tst_tree.hpp>
#include <tst_test.hpp>
#include <io_functions.h>
#include <bitset>
#include <cassert>
#include "cmdline.h"
#include "my_vector.h"
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
