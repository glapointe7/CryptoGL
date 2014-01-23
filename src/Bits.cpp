#include "Bits.hpp"

BytesVector Bits::rotateLeft128(const BytesVector &bytes, const uint8_t shift)
{
   const uint8_t bytes_to_rotate = shift / 8;
   const uint8_t bits_to_shift = shift % 8;
   const uint8_t rest = 8 - bits_to_shift;
   const uint64_t bytes_size = bytes.size();

   BytesVector rotated_bytes;
   rotated_bytes.reserve(bytes_size);
   for (uint8_t i = 0; i < bytes_size; ++i) 
   {
      rotated_bytes.push_back((bytes[(i+bytes_to_rotate) % bytes_size] << bits_to_shift) 
                            | (bytes[(i+bytes_to_rotate+1) % bytes_size] >> rest));
   }

   return rotated_bytes;
}

uint32_t Bits::bytesSwap(const uint32_t value)
{
   uint32_t retval = value & 0xFF;
   retval = (retval << 8) | ((value >> 8) & 0xFF);
   retval = (retval << 8) | ((value >> 16) & 0xFF);
   retval = (retval << 8) | ((value >> 24) & 0xFF);

   return retval;
}

/*uint32_t Bits::permute(const uint32_t value, const uint8_t *table)
{
   uint32_t result = 0;
   for(uint8_t i = 0; i < 32; ++i)
   {
      result |= setBitAtPosition(table[i], getBitAtPosition(i, value));
   }
   
   return result;
}*/

uint8_t Bits::msb(const BytesVector &V)
{
   return *(V.begin()) & 0x80;
}

uint8_t Bits::lsb(const BytesVector &V)
{
   return *(V.rbegin()) & 0x01;
}