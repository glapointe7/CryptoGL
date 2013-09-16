
#ifndef LITTLEENDIAN_HPP
#define LITTLEENDIAN_HPP

#include <vector>

#include "Types.hpp"

template <class UInt>
class LittleEndian
{
public:
   static_assert(std::is_integral<UInt>::value, "Type UInt must be an integral type.");
   
   static const BytesVector toBytesVector(const UInt &value)
   {
      BytesVector bytes;
      bytes.reserve(sizeof(UInt));
      for(uint8_t i = 0; i < sizeof(UInt) << 3; i += 8)
      {
         bytes.push_back((value >> i) & 0xFF);
      }
      
      return bytes;
   }
   
   static const UInt toInteger(const BytesVector &bytes)
   {
      UInt value = 0;
      for(uint8_t i = 0; i < sizeof(UInt); ++i)
      {
         value |= (static_cast<UInt>(bytes[i]) << (i << 3));
      }

      return value; 
   }
};

template <>
class LittleEndian<uint8_t>
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
class LittleEndian<uint16_t>
{
public:
   static const BytesVector toBytesVector(const uint16_t value)
   {
      return {static_cast<uint8_t>(value & 0xFF), 
              static_cast<uint8_t>(value >> 8)};
   }
   
   static const uint16_t toInteger(const BytesVector &bytes)
   {
      return bytes[0] | (bytes[1] << 8);
   }
};

template <>
class LittleEndian<uint32_t>
{
public:
   static const BytesVector toBytesVector(const uint32_t value)
   {
      return {static_cast<uint8_t>(value & 0xFF), 
              static_cast<uint8_t>((value >> 8) & 0xFF), 
              static_cast<uint8_t>((value >> 16) & 0xFF), 
              static_cast<uint8_t>(value >> 24)};
   }
   
   static const uint32_t toInteger(const BytesVector &bytes)
   {
      return  bytes[0]
              | (bytes[1] << 8) 
              | (bytes[2] << 16) 
              | (bytes[3] << 24);
   }
};

template <>
class LittleEndian<uint64_t>
{
public:
   static const BytesVector toBytesVector(const uint64_t &value)
   {
      return {static_cast<uint8_t>(value & 0xFF), 
              static_cast<uint8_t>((value >> 8) & 0xFF), 
              static_cast<uint8_t>((value >> 16) & 0xFF), 
              static_cast<uint8_t>((value >> 24) & 0xFF), 
              static_cast<uint8_t>((value >> 32) & 0xFF), 
              static_cast<uint8_t>((value >> 40) & 0xFF), 
              static_cast<uint8_t>((value >> 48) & 0xFF), 
              static_cast<uint8_t>(value >> 56)};
   }
   
   static const uint64_t toInteger(const BytesVector &bytes)
   {
      return  static_cast<uint64_t>(bytes[0])
              | (static_cast<uint64_t>(bytes[1]) << 8)
              | (static_cast<uint64_t>(bytes[2]) << 16)
              | (static_cast<uint64_t>(bytes[3]) << 24)
              | (static_cast<uint64_t>(bytes[4]) << 32)
              | (static_cast<uint64_t>(bytes[5]) << 40)
              | (static_cast<uint64_t>(bytes[6]) << 48)
              | (static_cast<uint64_t>(bytes[7]) << 56);
   }
};

using LittleEndian8 = LittleEndian<uint8_t>;
using LittleEndian16 = LittleEndian<uint16_t>;
using LittleEndian32 = LittleEndian<uint32_t>;
using LittleEndian64 = LittleEndian<uint64_t>;

#endif