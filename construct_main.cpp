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
    p.add<string>("output_file2", 'u', "output2 file name", false);

    p.add<string>("qgram", 'q', "qgram length", true);
    p.add<string>("count", 'a', "append count vector?", false);

    p.parse_check(argc, argv);
    string file = p.get<string>("input_file");
    string output_file = p.get<string>("output_file");
    string output_file2 = p.get<string>("output_file2");

    int32_t qgram = std::stoi(p.get<string>("qgram"));
    int32_t count = std::stoi(p.get<string>("count"));

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

    if (count == 1)
    {
        vector<uint64_t> vec;
        tree.translatePattern(itext, vec, true);
        tree.constructCountNonLeaveVec(vec);
    }

    tree.save(os);
    os.close();

    std::cout << "Finish." << std::endl;
}
