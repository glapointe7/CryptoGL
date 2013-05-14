#ifndef CONVERTER_TOOLS_HPP
#define CONVERTER_TOOLS_HPP

#include <string>
#include <vector>

std::string baseConvertString(const std::string &number, const unsigned char from_base,
        const unsigned char to_base);

void uppercase(std::string &text);
void convertMajMinToSymbol(std::string &text, const std::string symbol);
const std::vector<bool> convertBytesToBinary(const std::vector<unsigned char> &bytes);

std::string convertBytesToBinaryString(const std::vector<unsigned char> &bytes);

template <class Type>
Type lexical_cast(const std::string &textNumber);

#endif