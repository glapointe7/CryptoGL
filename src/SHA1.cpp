
#include "SHA1.hpp"

#include "Tools.hpp"

uint32_t SHA1::F(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t SHA1::G(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (x & z) | (y & z);
}

uint32_t SHA1::H(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return x ^ y ^ z;
}

const HashFunction::BytesContainer SHA1::encode(const BytesContainer &data)
{
   BytesContainer bits(appendPadding(data, Endianness::big_endian_64bits));
   const uint64_t bits_len = bits.size();

   /* States initialized in LITTLE ENDIAN. */
   WordsContainer state = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0};
   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getBigEndianWordBlocks(bits, i);
      
      // Extention of the 32-bits 16 blocks in 80 blocks of 32 bits.
      words.reserve(64);
      for (uint8_t j = 16; j < 80; ++j)
      {
         words.push_back(rotateLeft(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1, 32));
      }
      
      WordsContainer hash(state);
      uint32_t f, k;
      for (uint8_t j = 0; j < 80; ++j)
      {
         if (j < 20)
         {
            f = F(hash[1], hash[2], hash[3]);
            k = 0x5A827999;
         }
         else if (j < 40)
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0x6ED9EBA1;
         }
         else if (j < 60)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = 0x8F1BBCDC;
         }
         else
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0xCA62C1D6;
         }

         const uint32_t tmp = rotateLeft(hash[0], 5, 32) + f + hash[4] + k + words[j];
         hash[4] = hash[3];
         hash[3] = hash[2];
         hash[2] = rotateLeft(hash[1], 30, 32);
         hash[1] = hash[0];
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 5; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getBigEndianOutput(5, state);
}