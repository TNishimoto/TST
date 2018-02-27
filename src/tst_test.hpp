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
#include <set>
#include "tst_tree.hpp"

namespace tst{
	
	
	void computeAllQgrams(string &text, int q, vector<string> result);
	bool test(string& text, int q);
}