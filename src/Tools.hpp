
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>

#include "Types.hpp"

namespace Tools
{
   const std::vector<std::string> split(const std::string &text);
   void convertMajMinToSymbol(std::string &text, const std::string symbol);

   const uint64_t getBitsFromTable(const uint64_t &data, const uint8_t *table, const uint8_t from, const uint8_t to);
   const BytesVector getXORedBlock(const BytesVector &block1, const BytesVector &block2);

   const std::string baseXtoBaseY(const std::string &number, const uint8_t from_base, const uint8_t to_base);
}

#endif