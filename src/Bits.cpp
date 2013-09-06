#include "Bits.hpp"

const BytesVector Bits::rotateLeft128(const BytesVector &bytes, const uint8_t shift)
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