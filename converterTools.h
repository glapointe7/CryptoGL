#ifndef CONVERTER_TOOLS_H
#define CONVERTER_TOOLS_H

#include <string>

std::string baseConvertString(const std::string &number, const unsigned char from_base,
        const unsigned char to_base);
std::string convertTextToBinaryString(const std::string &text);
void uppercase(std::string &text);
void convertMajMinToSymbol(std::string &text, const std::string symbol);

template <class Type>
Type lexical_cast(const std::string &textNumber);

#endif