#pragma once
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
using namespace std;
namespace my {
    template <typename T>
	static void substr(vector<T> &text, uint64_t b, uint64_t len, vector<T> &output)
	{
        output.resize(len);
        for(uint64_t i=0;i<len;i++){
            output[i] = text[b+i];
        }
	}
    template <typename T>
	static void concat(vector<T> &text, vector<T> &pattern)
	{
        for(uint64_t i=0;i<pattern.size();i++){
            text.push_back(pattern[i]);
        }        
	}
    static void createFromString(std::string& text, vector<uint8_t> &output)
	{
        for(uint64_t i=0;i<text.size();i++){
            output.push_back(static_cast<uint8_t>(text[i]));
        }        
	}
    static string to_string(vector<uint8_t> &text)
	{
        string str;
        for(uint64_t i=0;i<text.size();i++){
            str += (char)text[i];
        }        
        return str;
	}
}