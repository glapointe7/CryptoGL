
#ifndef LITTLEENDIAN_HPP
#define	LITTLEENDIAN_HPP

#include "Endianness.hpp"

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

using LittleEndian32 = LittleEndian<uint32_t>;
using LittleEndian64 = LittleEndian<uint64_t>;

#endif