/*
 * Utilities for vectors.
 */
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Types.hpp"
#include "String.hpp"
#include "Integer.hpp"

#include <functional>

/* Extension of std::vector. */
template <typename Type>
class Vector1
{
private:
   using VectorType = std::vector<Type>;
   using MatrixType = std::vector<VectorType>;
   VectorType V;
   
public:
   explicit Vector1(const uint64_t &new_size) { V.reserve(new_size); }
   
   /* XOR each element of vector V with the elements of vector W and return the result. */
   VectorType Xor(const VectorType &W)
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
   
   /* Append a value at the end of the vector. */
   inline void append(const Type value)
   {
      V.push_back(value);
   }
   
   /* Return a vector containing a range of values in vector V from begin to end. */
   VectorType range(const uint64_t &begin, const uint64_t &end)
   {
      return VectorType(V.begin() + begin, V.begin() + end);
   }
   
   /* Return a vector containing a range of values in vector V from begin to end of V. */
   VectorType range(const uint64_t &begin)
   {
      return VectorType(V.begin() + begin, V.end());
   }
   
   /* Append the vector W at the end of V. */
   void extend(VectorType W)
   {
      V.insert(V.end(), W.begin(), W.end());
   }
   
   /* Append the range of W given from begin to end at the end. */
   void extend(VectorType W, const uint64_t &begin, const uint64_t &end)
   {
      V.insert(V.end(), W.begin() + begin, W.begin() + end);
   }
   
   /* Append the range of W given from begin to end at the end of V. */
   void extend(VectorType W, const uint64_t &begin)
   {
      V.insert(V.end(), W.begin() + begin, W.end());
   }
   
   /* Add the vector W at the end of V and reserve new capacity. Return the new merged vector. */
   void merge(const VectorType &W)
   {
      V.reserve(V.size() + W.size());
      extend(V, W);
   }
   
   /* Find a value in the vector and return the position if found, -1 if not. */
   int64_t find(const Type value)
   {
      uint64_t i = 0;
      for(const auto val : V)
      {
         if(val == value)
         {
            return i;
         }
         i++;
      }
      
      return -1;
   }
   
   /* Split the vector V in 'vector_size' chunks and return the vector of chunks. */
   MatrixType chunk(const uint64_t &vector_size)
   {
      if(V.empty())
      {
         return {};
      }
      
      MatrixType result;
      const uint64_t V_size = V.size();
      const uint64_t matrix_size = V_size / vector_size;
     
      result.reserve(matrix_size);
      for(uint64_t i = 0; i < V_size; i += vector_size)
      {
         result.push_back(range(i, i + vector_size));
      }

      return result;
   }
   
   /* Transform a vector of integers to a hexadecimal string. */
   std::string toHexString()
   {
      const uint8_t data_size = sizeof(typename VectorType::value_type) * 8;
      std::string hex_digest;
      hex_digest.reserve(V.size() * data_size / 4);
      for(const auto uint : V)
      {
         for(int8_t i = data_size - 4; i >= 0; i -= 4)
         {
            hex_digest.push_back(String::hex_digits[(uint >> i) % 16]);
         }
      }

      return hex_digest;
   }
   
   /* Convert a Vector to a string. *********ADD Reserve********/
   std::string toString()
   {
      std::string result;
      for(const auto number : V)
      {
         result.append(Integer<Type>::toString(number));
      }
      
      return result;
   }
};

using VectorOfBytes = Vector1<uint8_t>;
using VectorOfUInt16 = Vector1<uint16_t>;
using VectorOfUInt32 = Vector1<uint32_t>;
using VectorOfUInt64 = Vector1<uint64_t>;
using VectorOfStrings = Vector1<std::string>;




namespace Vector
{
   /*
    * Lambda Shift classes are redondants. The best way is to pass the operator << or >>
    * as an argument to a function like this pseudo-code : shift(5, <<);
    */
   uint8_t shr(uint8_t value, const uint8_t to_shift);
   uint8_t shl(uint8_t value, const uint8_t to_shift);
   using SH = std::function<uint8_t(uint8_t value, const uint8_t to_shift)>;
   
   class Shift
   {
   public:
      Shift(const uint32_t to_shift, const SH &sh, const SH &sh_inv) 
         : carry(0), sh(sh), sh_inv(sh_inv), mask(sh(0xFF, 8 - to_shift)), 
           shift(to_shift), inner_shift(8 - to_shift) {}
      virtual ~Shift() {}
      
      uint8_t operator()(uint8_t value)
      {
         const uint8_t direction = value & mask;
         value = sh(value, shift);
         value |= carry;

         carry = sh_inv(direction, inner_shift);
         return value;
      }
      
   private:
      uint8_t carry;
      const SH sh, sh_inv;
      const uint8_t mask;
      const uint32_t shift;
      const uint32_t inner_shift;
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
   
   /* Return a vector containing a range of values in vector V from begin to end of V. */
   template <class VectorType>
   VectorType range(const VectorType &V, const uint64_t &begin, const uint64_t &end)
   {
      return VectorType(V.begin() + begin, V.begin() + end);
   }
   
   /* Return a vector containing a range of values in vector V from begin to end. */
   template <class VectorType>
   VectorType range(const VectorType &V, const uint64_t &begin)
   {
      return VectorType(V.begin() + begin, V.end());
   }
   
   /* Append the vector W at the end of V. */
   template <class VectorType>
   void extend(VectorType &self, const VectorType &V)
   {
      self.insert(self.end(), V.begin(), V.end());
   }
   
   /* Append the range of W given from begin to end at the end of V. */
   template <class VectorType>
   void extend(VectorType &self, const VectorType &V, const uint64_t &begin, const uint64_t &end)
   {
      self.insert(self.end(), V.begin() + begin, V.begin() + end);
   }
   
   /* Append the range of W given from begin to end at the end of V. */
   template <class VectorType>
   void extend(VectorType &self, const VectorType &V, const uint64_t &begin)
   {
      self.insert(self.end(), V.begin() + begin, V.end());
   }
   
   /* Add the vector W at the end of V and return the new merged vector. */
   template <class VectorType>
   VectorType merge(VectorType V, const VectorType &W)
   {
      V.reserve(V.size() + W.size());
      extend(V, W);
      
      return V;
   }
   
   /* Split the vector V in 'vector_size' chunks and return the vector of chunks. */
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
     
      result.reserve(matrix_size);
      for(uint64_t i = 0; i < V_size; i += vector_size)
      {
         result.push_back(VectorType(V.begin() + i, V.begin() + i + vector_size));
      }

      return result;
   }
   
   /* Transform a vector of integers to a hexadecimal string. */
   template <class DataTypeVector>
   std::string toHexString(const DataTypeVector &bytes)
   {
      const uint8_t data_size = sizeof(typename DataTypeVector::value_type) * 8;
      std::string hex_digest;
      hex_digest.reserve(bytes.size() * data_size / 4);
      for(const auto uint : bytes)
      {
         for(int8_t i = data_size - 4; i >= 0; i -= 4)
         {
            hex_digest.push_back(String::hex_digits[(uint >> i) & 0xF]);
         }
      }

      return hex_digest;
   }
   
   /* Convert a bytes Vector to a string. */
   std::string toString(const BytesVector &bytes);
   
   /* Shift a vector of bytes 'v' to the left of 'shift' bits. */
   BytesVector leftShift(const BytesVector &v, uint32_t shift);
   
   /* Shift a vector of bytes 'v' to the right of 'shift' bits. */
   BytesVector rightShift(const BytesVector &v, const uint32_t shift);
   
   /* Rotate a vector of bytes 'V' to the left of 'to_rotate' bits. */
   BytesVector rotateLeft(BytesVector V, const uint32_t to_rotate);
   
   /* Rotate a vector of bytes 'V' to the right of 'to_rotate' bits. */
   BytesVector rotateRight(BytesVector V, const uint32_t to_rotate);
   
   /* Split the string str with a specific separator. */
   std::vector<std::string> split(const std::string &str, const int8_t separator);
}

#endif