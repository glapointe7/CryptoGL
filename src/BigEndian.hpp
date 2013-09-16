
#ifndef BIGENDIAN_HPP
#define BIGENDIAN_HPP

#include <vector>

#include "Types.hpp"

template <class UInt>
class BigEndian
{
public:
   static_assert(std::is_integral<UInt>::value, "Type UInt must be an integral type.");
   
   static const UInt toInteger(const BytesVector &bytes)
   {
      UInt value = 0;
      for(uint8_t i = 0; i < sizeof(UInt); ++i)
      {
         value |= (static_cast<UInt>(bytes[i]) << ((sizeof(UInt) - i - 1) << 3));
      }

      return value; 
   }
   
   static const BytesVector toBytesVector(const UInt &value)
   {
      BytesVector bytes;
      bytes.reserve(sizeof(UInt));
      for(int8_t i = (sizeof(UInt) - 1) << 3; i >= 0; i -= 8)
      {
         bytes.push_back((value >> i) & 0xFF);
      }
      
      return bytes;
   }
};

template <>
class BigEndian<uint8_t>
{
public:
   static const BytesVector toBytesVector(const uint8_t value)
   {
      return {value};
   }
   
   static const uint8_t toInteger(const BytesVector &bytes)
   {
      return bytes[0];
   }
};

template <>
class BigEndian<uint16_t>
{
public:
   static const BytesVector toBytesVector(const uint16_t value)
   {
      return {static_cast<uint8_t>(value >> 8), 
              static_cast<uint8_t>(value & 0xFF)};
   }
   
   static const uint16_t toInteger(const BytesVector &bytes)
   {
      return (bytes[0] << 8) | bytes[1];
   }
};

template <>
class BigEndian<uint32_t>
{
public:
   static const BytesVector toBytesVector(const uint32_t value)
   {
      return {static_cast<uint8_t>(value >> 24), 
              static_cast<uint8_t>((value >> 16) & 0xFF), 
              static_cast<uint8_t>((value >> 8) & 0xFF), 
              static_cast<uint8_t>(value & 0xFF)};
   }
   
   static const uint32_t toInteger(const BytesVector &bytes)
   {
      return (bytes[0] << 24) 
              | (bytes[1] << 16) 
              | (bytes[2] << 8) 
              | bytes[3];
   }
};

template <>
class BigEndian<uint64_t>
{
public:
   static const BytesVector toBytesVector(const uint64_t &value)
   {
      return {static_cast<uint8_t>(value >> 56), 
              static_cast<uint8_t>((value >> 48) & 0xFF), 
              static_cast<uint8_t>((value >> 40) & 0xFF), 
              static_cast<uint8_t>((value >> 32) & 0xFF), 
              static_cast<uint8_t>((value >> 24) & 0xFF), 
              static_cast<uint8_t>((value >> 16) & 0xFF), 
              static_cast<uint8_t>((value >> 8) & 0xFF), 
              static_cast<uint8_t>(value & 0xFF)};
   }
   
   static const uint64_t toInteger(const BytesVector &bytes)
   {
      return (static_cast<uint64_t>(bytes[0]) << 56) 
              | (static_cast<uint64_t>(bytes[1]) << 48) 
              | (static_cast<uint64_t>(bytes[2]) << 40) 
              | (static_cast<uint64_t>(bytes[3]) << 32)
              | (static_cast<uint64_t>(bytes[4]) << 24)
              | (static_cast<uint64_t>(bytes[5]) << 16)
              | (static_cast<uint64_t>(bytes[6]) << 8)
              | static_cast<uint64_t>(bytes[7]);
   }
};

using BigEndian8 = BigEndian<uint8_t>;
using BigEndian16 = BigEndian<uint16_t>;
using BigEndian32 = BigEndian<uint32_t>;
using BigEndian64 = BigEndian<uint64_t>;

#endif