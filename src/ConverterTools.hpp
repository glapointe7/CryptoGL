#ifndef CONVERTER_TOOLS_HPP
#define CONVERTER_TOOLS_HPP

#include <string>
#include <vector>
#include <sstream>

const std::string baseConvertString(const std::string &number, const uint8_t from_base,
        const uint8_t to_base);

void convertMajMinToSymbol(std::string &text, const std::string symbol);
const std::vector<bool> convertBytesToBinary(const std::vector<uint8_t> &bytes);
std::string convertBytesToBinaryString(const std::vector<uint8_t> &bytes);

/*template <class Type>
const Type lexical_cast(const std::string &textNumber)
{
   std::istringstream ss(textNumber);

   Type number;
   ss >> number;

   return number;
}*/

#endif