#include "Bits.hpp"

BytesVector Bits::rotateLeft128(const BytesVector &bytes, const uint8_t shift)
{
   const uint8_t w = shift >> 3;
   const uint8_t b = shift & 7;

   BytesVector rotated_bytes;
   rotated_bytes.reserve(16);
   for (uint8_t i = 0; i < 16; ++i) {
      rotated_bytes.push_back((bytes[(i+w) & 0xF] << b) | (bytes[(i+w+1) & 0xF] >> (8 - b)));
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

uint32_t Bits::permute(const uint32_t value, const uint8_t *table)
{
   uint32_t result = 0;
   for(uint8_t i = 0; i < 32; ++i)
   {
      result |= setBitAtPosition(table[i], getBitAtPosition(i, value));
   }
   
   return result;
}