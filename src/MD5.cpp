#include "MD5.hpp"

#include "Tools.hpp"  // rotateLeft

const HashFunction::BytesContainer MD5::left_rotation_table = {
   7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
   5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
   4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
   6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const HashFunction::WordsContainer MD5::sine_magic_numbers = {
   0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
   0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
   0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
   0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
   0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
   0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
   0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
   0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
   0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
   0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
   0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
   0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
   0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t MD5::F(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t MD5::G(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & z) | (y & ~z);
}

uint32_t MD5::H(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return x ^ y ^ z;
}

uint32_t MD5::I(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return y ^ (x | ~z);
}

const HashFunction::BytesContainer MD5::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data, Endianness::little_endian));
   const uint64_t bytes_len = bytes.size();

   /* Initial values. */
   WordsContainer state = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
   
   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      WordsContainer words = getLittleEndianWordBlocks(bytes, i);
      WordsContainer hash(state);
      uint32_t f, k;
      for (uint8_t j = 0; j < 64; ++j)
      {
         if (j < 16)
         {
            f = F(hash[1], hash[2], hash[3]);
            k = j;
         }
         else if (j < 32)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = ((5 * j) + 1) & 0xF;
         }
         else if (j < 48)
         {
            f = H(hash[1], hash[2], hash[3]);
            k = ((3 * j) + 5) & 0xF;
         }
         else
         {
            f = I(hash[1], hash[2], hash[3]);
            k = (7 * j) & 0xF;
         }

         const uint32_t tmp = hash[3];
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] += rotateLeft(hash[0] + f + words[k] + sine_magic_numbers[j], left_rotation_table[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getLittleEndianOutput(4, state);
}