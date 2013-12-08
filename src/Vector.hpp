/*
 * Utilities for vectors.
 */
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Types.hpp"

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
   std::vector<VectorType> chunk(const VectorType &V, const uint64_t &vector_size)
   {
      if(V.empty())
      {
         return {};
      }
      
      std::vector<VectorType> result;
      const uint64_t V_size = V.size();
      const uint64_t matrix_size = V_size / vector_size;
      //const uint64_t matrix_size = static_cast<uint64_t>(ceil(static_cast<float>(V_size) / vector_size));
      result.reserve(matrix_size);
      for(uint64_t i = 0; i < V_size; i += vector_size)
      {
         result.push_back(VectorType(V.begin() + i, V.begin() + i + vector_size));
      }

      return result;
   }
   
   /* Shift a vector of bytes 'v' to the left of 'shift' bits. */
   BytesVector leftShift(const BytesVector &v, uint32_t shift);
   
   /* Shift a vector of bytes 'v' to the right of 'shift' bits. */
   BytesVector rightShift(const BytesVector &v, const uint32_t shift);
}

#endif