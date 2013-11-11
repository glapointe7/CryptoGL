
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>
#include <array>

#include "Types.hpp"

namespace Tools
{
   std::vector<std::string> split(const std::string &text);
   void convertMajMinToSymbol(std::string &text, const std::string symbol);

   BytesVector getXORedBlock(const BytesVector &block1, const BytesVector &block2);

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
   
   /* XOR each element of vector V with the elements of vector W and return the result. */
   template <class VectorType>
   VectorType XORVectors (const VectorType &V, const VectorType &W)
   {
      const uint32_t size = V.size();
      VectorType out;
      out.reserve(size);

      for(uint32_t i = 0; i < size; ++i)
      {
         out.push_back(V[i] ^ W[i]);
      }

      return out;
   }
   
   template <class VectorType>
   VectorType mergeVectors(const VectorType &V, const VectorType &W)
   {
      VectorType result(V);
      result.reserve(V.size() + W.size());
      result.insert(result.end(), W.begin(), W.end());
      
      return result;
   }
}

#endif