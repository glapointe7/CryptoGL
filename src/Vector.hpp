/*
 * Utilities for vectors.
 */
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Types.hpp"

namespace Vector
{
   class LambdaShift
   {
   protected:
      explicit LambdaShift(const uint32_t shift)
         : carry(0), shift(shift), inner_shift(8 - shift) {}
         
      virtual ~LambdaShift() {}
      virtual uint8_t operator()(uint8_t val) = 0;
      
      uint8_t carry;
      const uint32_t shift;
      const uint32_t inner_shift;
   };
   
   /* Used as a lambda function to right shift each element of a vector. */
   class LambdaRightShift : public LambdaShift
   {
   public:
      explicit LambdaRightShift(const uint32_t shift)
         : LambdaShift(shift), mask(0xFF >> (8 - shift)) {}

      uint8_t operator()(uint8_t val) final
      {
         const uint8_t right = val & mask;
         val >>= shift;
         val |= carry;

         carry = right << inner_shift;
         return val;
      }
      
   private:
      const uint8_t mask;
   };
   
   /* Used as a lambda function to left shift each element of a vector. */
   class LambdaLeftShift : public LambdaShift
   {
   public:
      explicit LambdaLeftShift(const uint32_t shift)
         : LambdaShift(shift), mask(0xFF << (8 - shift)) {}

      uint8_t operator()(uint8_t val) final
      {
         const uint8_t left = val & mask;
         val <<= shift;
         val |= carry;

         carry = left >> inner_shift;
         return val;
      }
      
   private:
      const uint8_t mask;
   };
   
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
   VectorType merge(VectorType V, const VectorType &W)
   {
      VectorType result(V);
      result.reserve(V.size() + W.size());
      result.insert(result.end(), W.begin(), W.end());
      
      return result;
   }
   
   /* Add the vector W at the end of V and return the new merged vector. */
   template <class VectorType>
   void extend(VectorType &self, const VectorType &V)
   {
      self.insert(self.end(), V.begin(), V.end());
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