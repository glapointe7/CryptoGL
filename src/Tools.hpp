
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Types.hpp"

#include <array>

namespace Tools
{
   std::vector<std::string> split(const std::string &text);
   
   void convertMajMinToSymbol(std::string &text, const std::string symbol);

   std::string baseXtoBaseY(const std::string &number, const uint8_t from_base, const uint8_t to_base);
   
   template <uint8_t TableSize>
   uint64_t getBitsFromTable(const uint64_t &data, const std::array<uint8_t, TableSize> &table, const uint8_t from)
   {
      uint8_t i = TableSize;
      uint64_t output = 0;

      for (uint8_t k = 0; k < TableSize; ++k)
      {
         --i;
         if ((data >> (from - table[k])) & 0x1)
         {
            output |= 1ull << i;
         }
      }

      return output;
   }
}

#endif