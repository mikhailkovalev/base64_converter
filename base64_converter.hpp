#ifndef BASE64_CONVERTER_HPP_INCLUDED
#define BASE64_CONVERTER_HPP_INCLUDED

#include <fstream>
//#include <iostream>

namespace Base64_converter
{
    bool toBase64(const char* inputFileName, const char* outputFileName);
    bool fromBase64(const char* inputFileName, const char* outputFileName);
    bool isCharValid(char c);
}

#endif // BASE64_CONVERTER_HPP_INCLUDED
