
#ifndef LITTLEENDIAN_HPP
#define LITTLEENDIAN_HPP

#include "Types.hpp"
#include "Vector.hpp"

template <typename UInt>
class LittleEndian
{
static_assert(std::is_integral<UInt>::value, "Type UInt must be an integral type.");
using UIntVector = std::vector<UInt>;

public:   
   static BytesVector toBytesVector(const UInt &value)
   {
      BytesVector bytes;
      const uint8_t size_UInt_8 = sizeof(UInt) * 8;
      bytes.reserve(sizeof(UInt));
      for(uint8_t i = 0; i < size_UInt_8; i += 8)
      {
         bytes.push_back((value >> i) & 0xFF);
      }
      
      return bytes;
   }
   
   static UInt toInteger(const BytesVector &bytes)
   {
      UInt value = 0;
      for(uint8_t i = 0; i < sizeof(UInt); ++i)
      {
         value |= (static_cast<UInt>(bytes[i]) << (i << 3));
      }

      return value; 
   }
   
   /* Convert a vector to an integer from a range(from, to). */
   static UInt toIntegerRange(const BytesVector &bytes, const uint8_t from, const uint8_t to)
   {
      return toInteger(Vector::range(bytes, from, to));
   }
   
   /* Convert a vector to an integer from a range(0, to). */
   static UInt toIntegerRange(const BytesVector &bytes, const uint8_t from)
   {
      return toInteger(Vector::range(bytes, from));
   }
   
   static UIntVector toIntegersVector(const BytesVector &V)
   {
      const uint8_t BlockSize = V.size();
      const uint8_t type_size = sizeof(UInt);
      UIntVector result;
      result.reserve(BlockSize / type_size);
      for(uint8_t i = 0; i < BlockSize; i += type_size)
      {
         result.push_back(toIntegerRange(V, i, i + type_size));
      }

      return result;
   }
   
   static BytesVector toBytesVector(const UIntVector &V)
   {
      const uint8_t BlockSize = V.size();
      BytesVector result;
      result.reserve(BlockSize * sizeof(UInt));
      for(uint8_t i = 0; i < BlockSize; ++i)
      {
         Vector::extend(result, toBytesVector(V[i]));
      }

      return result;
   }
};

using LittleEndian8 = LittleEndian<uint8_t>;
using LittleEndian16 = LittleEndian<uint16_t>;
using LittleEndian32 = LittleEndian<uint32_t>;
using LittleEndian64 = LittleEndian<uint64_t>;

#endif