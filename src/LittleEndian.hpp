
#ifndef LITTLEENDIAN_HPP
#define LITTLEENDIAN_HPP

#include <vector>

class LittleEndian
{
public:
   typedef std::vector<uint8_t> BytesVector;
   static const BytesVector toBytesVector(const uint16_t value)
   {
      return {value & 0xFF, 
              value >> 8};
   }
   
   static const BytesVector toBytesVector(const uint32_t value)
   {
      return {value & 0xFF, 
              (value >> 8) & 0xFF, 
              (value >> 16) & 0xFF, 
              value >> 24};
   }
   
   static const BytesVector toBytesVector(const uint64_t &value)
   {
      return {value & 0xFF, 
              (value >> 8) & 0xFF, 
              (value >> 16) & 0xFF, 
              (value >> 24) & 0xFF, 
              (value >> 32) & 0xFF, 
              (value >> 40) & 0xFF, 
              (value >> 48) & 0xFF, 
              value >> 56};
   }
   
   static const uint16_t toInteger(const BytesVector &bytes)
   {
      return bytes[0] 
              | (bytes[1] << 8);
   }
   
   static const uint32_t toInteger(const BytesVector &bytes)
   {
      return  bytes[0]
              | (bytes[1] << 8) 
              | (bytes[2] << 16) 
              | (bytes[3] << 24);
   }
   
   static const uint64_t toInteger(const BytesVector &bytes)
   {
      return  bytes[0]
              | (bytes[1] << 8)
              | (bytes[2] << 16)
              | (bytes[3] << 24)
              | (static_cast<uint64_t>(bytes[4]) << 32)
              | (static_cast<uint64_t>(bytes[5]) << 40)
              | (static_cast<uint64_t>(bytes[6]) << 48)
              | (static_cast<uint64_t>(bytes[7]) << 56);
   }
};


/*
template <class UInt>
class LittleEndian : public Endianness<UInt>
{
public:
   virtual void toBytes(const UInt &word) final
   {
      for(uint8_t i = 0; i < this->int_size << 3; i += 8)
      {
         this->bytes[i >> 3] = (word >> i) & 0xFF;
      }
   }
   
   virtual void toInteger(const typename Endianness<UInt>::BytesContainer &bytes) final
   {
      for(uint8_t i = 0; i < this->int_size; ++i)
      {
         this->value |= (static_cast<UInt>(bytes[i]) << (i << 3));
      }
   }
};

using LittleEndian16 = LittleEndian<uint16_t>;
using LittleEndian32 = LittleEndian<uint32_t>;
using LittleEndian64 = LittleEndian<uint64_t>;*/

#endif