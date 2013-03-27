#ifndef CONVERTER_TOOLS_H
#define CONVERTER_TOOLS_H

#include <string>

using namespace std;

string baseConvertString(const string number, const unsigned char from_base,
        const unsigned char to_base);
string convertTextToBinaryString(const string &text);
void convertTextToMajus(string &text);

template <class Type>
Type lexical_cast(const string &textNumber);

#endif