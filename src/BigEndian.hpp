
#ifndef BIGENDIAN_HPP
#define BIGENDIAN_HPP

#include <vector>

class BigEndian
{
public:
   typedef std::vector<uint8_t> BytesVector;
   static const BytesVector toBytesVector(const uint16_t value)
   {
      return {value >> 8, 
              value & 0xFF};
   }
   
   static const BytesVector toBytesVector(const uint32_t value)
   {
      return {value >> 24, 
              (value >> 16) & 0xFF, 
              (value >> 8) & 0xFF, 
              value & 0xFF};
   }
   
   static const BytesVector toBytesVector(const uint64_t &value)
   {
      return {value >> 56, 
              (value >> 48) & 0xFF, 
              (value >> 40) & 0xFF, 
              (value >> 32) & 0xFF, 
              (value >> 24) & 0xFF, 
              (value >> 16) & 0xFF, 
              (value >> 8) & 0xFF, 
              value & 0xFF};
   }
   
   static const uint16_t toInteger(const BytesVector &bytes)
   {
      return (bytes[0] << 8) 
              | bytes[1];
   }
   
   static const uint32_t toInteger(const BytesVector &bytes)
   {
      return (bytes[0] << 24) 
              | (bytes[1] << 16) 
              | (bytes[2] << 8) 
              | bytes[3];
   }
   
   static const uint64_t toInteger(const BytesVector &bytes)
   {
      return (static_cast<uint64_t>(bytes[0]) << 56) 
              | (static_cast<uint64_t>(bytes[1]) << 48) 
              | (static_cast<uint64_t>(bytes[2]) << 40) 
              | (static_cast<uint64_t>(bytes[3]) << 32)
              | (bytes[4] << 24)
              | (bytes[5] << 16)
              | (bytes[6] << 8)
              | bytes[7];
   }
};



/*template <class UInt>
class BigEndian : public Endianness<UInt>
{
public:   
   virtual void toBytes(const UInt &word) final
   {
      const uint8_t max = (this->int_size - 1) << 3;
      for(int8_t i = max; i >= 0; i -= 8)
      {
         this->bytes[(max - i) >> 3] = (word >> i) & 0xFF;
      }
   }
   
   virtual void toInteger(const typename Endianness<UInt>::BytesContainer &bytes) final
   {
      for(uint8_t i = 0; i < this->int_size; ++i)
      {
         this->value |= (static_cast<UInt>(bytes[i]) << ((this->int_size - i - 1) << 3));
      }
   }
};

using BigEndian16 = BigEndian<uint16_t>;
using BigEndian32 = BigEndian<uint32_t>;
using BigEndian64 = BigEndian<uint64_t>;*/

#endif