#include "MessageDigest.hpp"

#include "Bits.hpp"

constexpr uint8_t MD4::left_rotation_table[];
constexpr uint8_t MD4::word_indexes[];
constexpr uint32_t MD4::k[];
constexpr uint8_t MD5::left_rotation_table[];
constexpr uint32_t MD5::sine_magic_numbers[];

void MD4::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      const uint8_t index = j >> 4;
      uint32_t f;
      switch(index)
      {
         case 0: f = F(hash[1], hash[2], hash[3]); break;
         case 1: f = G(hash[1], hash[2], hash[3]); break;
         case 2: f = H(hash[1], hash[2], hash[3]); break;
      }
      
      const uint32_t tmp = hash[3];
      hash[3] = hash[2];
      hash[2] = hash[1];
      hash[1] = Bits::rotateLeft(hash[0] + f + int_block[word_indexes[j]] + k[index], left_rotation_table[j], 32);
      hash[0] = tmp;
   }

   for (uint8_t j = 0; j < 4; ++j)
   {
      state[j] += hash[j];
   }
}

void MD5::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      const uint8_t index = j >> 4;
      uint32_t f, k;
      switch(index)
      {
         case 0: f = F(hash[1], hash[2], hash[3]); k = j; break;
         case 1: f = G(hash[1], hash[2], hash[3]); k = ((5 * j) + 1) & 0xF; break;
         case 2: f = H(hash[1], hash[2], hash[3]); k = ((3 * j) + 5) & 0xF; break;
         case 3: f = I(hash[1], hash[2], hash[3]); k = (7 * j) & 0xF; break;
      }
      
      const uint32_t tmp = hash[3];
      hash[3] = hash[2];
      hash[2] = hash[1];
      hash[1] += Bits::rotateLeft(hash[0] + f + int_block[k] + sine_magic_numbers[j], left_rotation_table[j], 32);
      hash[0] = tmp;
   }

   for (uint8_t j = 0; j < 4; ++j)
   {
      state[j] += hash[j];
   }
}