
#ifndef BIGENDIAN_HPP
#define	BIGENDIAN_HPP

#include "Endianness.hpp"

template <class UInt>
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

using BigEndian32 = BigEndian<uint32_t>;
using BigEndian64 = BigEndian<uint64_t>;

#endif