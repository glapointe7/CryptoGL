
#ifndef BIGENDIAN_HPP
#define BIGENDIAN_HPP

#include "Types.hpp"
#include "Vector.hpp"

template <typename UInt>
class BigEndian
{
static_assert(std::is_integral<UInt>::value, "Type UInt must be an integral type.");
using UIntVector = std::vector<UInt>;

public:   
   static UInt toInteger(const BytesVector &bytes)
   {
      UInt value = 0;
      for(uint8_t i = 0; i < sizeof(UInt); ++i)
      {
         value |= (static_cast<UInt>(bytes[i]) << ((sizeof(UInt) - i - 1) * 8));
      }

      return value; 
   }
   
   /* Convert a vector to an integer from a range(from, to). */
   static UInt toIntegerRange(const BytesVector &bytes, const uint8_t from, const uint8_t to)
   {
      return toInteger(BytesVector(bytes.begin() + from, bytes.begin() + to));
   }
   
   /* Convert a vector to an integer from a range(0, to). */
   static UInt toIntegerRange(const BytesVector &bytes, const uint8_t to)
   {
      return toInteger(BytesVector(bytes.begin(), bytes.begin() + to));
   }
   
   static BytesVector toBytesVector(const UInt &value)
   {
      BytesVector bytes;
      bytes.reserve(sizeof(UInt));
      for(int8_t i = (sizeof(UInt) - 1) * 8; i >= 0; i -= 8)
      {
         bytes.push_back((value >> i) & 0xFF);
      }
      
      return bytes;
   }
   
   static UIntVector toIntegersVector(const BytesVector &V)
   {
      const uint8_t BlockSize = V.size();
      const uint8_t type_size = sizeof(UInt);
      UIntVector result;
      result.reserve(BlockSize / type_size);
      for(uint8_t i = 0; i < BlockSize; i += type_size)
      {
         result.push_back(toInteger(BytesVector(V.begin() + i, V.begin() + i + type_size)));
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

using BigEndian8 = BigEndian<uint8_t>;
using BigEndian16 = BigEndian<uint16_t>;
using BigEndian32 = BigEndian<uint32_t>;
using BigEndian64 = BigEndian<uint64_t>;

#endif