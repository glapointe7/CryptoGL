/*
 * Utilities for vectors.
 */
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Types.hpp"
#include <algorithm>

namespace Vector
{
   /* XOR each element of vector V with the elements of vector W and return the result. */
   template <class VectorType>
   VectorType Xor(const VectorType &V, const VectorType &W)
   {
      const uint64_t size = V.size();
      VectorType out;
      out.reserve(size);

      for(uint64_t i = 0; i < size; ++i)
      {
         out.push_back(V[i] ^ W[i]);
      }

      return out;
   }
   
   /* Add the vector W at the end of V and return the new merged vector. */
   template <class VectorType>
   VectorType merge(const VectorType &V, const VectorType &W)
   {
      VectorType result(V);
      result.reserve(V.size() + W.size());
      result.insert(result.end(), W.begin(), W.end());
      
      return result;
   }
   
   /* Split the vector V in 'new_size' chunks and return the vector of chunks. */
   template <class VectorType>
   std::vector<VectorType> chunk(const VectorType &V, const uint64_t &new_size)
   {
      std::vector<VectorType> result;
      result.reserve(new_size);
      const uint64_t V_size = V.size();
      const uint64_t step = V_size / new_size;
      for(uint64_t i = 0; i < V_size; i += step)
      {
         result.push_back(VectorType(V.begin() + i, V.begin() + i + step));
      }

      return result;
   }
   
   template <uint64_t Shift>
   BytesVector leftShift(const BytesVector &V)
   {      
      const uint64_t V_size = V.size();
      constexpr uint8_t rest = Shift & 7;
      constexpr uint64_t S = Shift >> 3;
      BytesVector result(V);
      
      std::rotate(result.begin(), result.begin() + S, result.end());
      const uint64_t V_minus_S = V_size - S;
      for(uint64_t i = V_size - 1; i >= V_minus_S; --i)
      {
         result[i] = 0;
      }
      
      uint8_t msb_byte = (result[V_minus_S - 1] >> (8-rest)) % (1 << rest);
      result[V_minus_S - 1] <<= rest;
      uint8_t next_msb_byte;
      for(uint64_t i = V_minus_S - 2; i >= 1; --i)
      {
         next_msb_byte = (result[i] >> (8-rest)) % (1 << rest);
         result[i] <<= rest;
         result[i] |= next_msb_byte;
         msb_byte = next_msb_byte;
      }
      result[0] <<= rest;
      result[0] |= msb_byte;
      
      return result;
   }
   
   template <uint64_t Shift>
   BytesVector rightShift(const BytesVector &V)
   {      
      const uint64_t V_size = V.size();
      constexpr uint8_t rest = Shift & 7;
      constexpr uint64_t S = Shift >> 3;
      BytesVector result(V);
      
      std::rotate(result.begin(), result.begin() + V_size - S, result.end());
      for(uint64_t i = 0; i < S; ++i)
      {
         result[i] = 0;
      }
      
      uint8_t lsb_byte = result[S] % (1 << rest);
      result[S] >>= rest;
      uint8_t next_lsb_byte;
      for(uint64_t i = S+1; i < V_size-1; ++i)
      {
         next_lsb_byte = result[i] % (1 << rest);
         result[i] >>= rest;
         result[i] |= (next_lsb_byte << (8-rest));
         lsb_byte = next_lsb_byte;
      }
      result[V_size - 1] >>= rest;
      result[V_size - 1] |= (lsb_byte << (8-rest));
      
      return result;
   }
}

#endif