#include "SHA1.hpp"

#include "Bits.hpp"

constexpr uint32_t SHA1::k[];

void SHA1::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   int_block.resize(rounds);

   // Extention of the 16 32-bit blocks in 80 blocks of 32 bits.
   for (uint8_t j = 16; j < rounds; ++j)
   {
      int_block[j] = Bits::rotateLeft(int_block[j - 3] ^ int_block[j - 8] ^ int_block[j - 14] ^ int_block[j - 16], 1, 32);
   }

   UInt32Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      const uint8_t index = j / 20;
      uint32_t f;
      switch(index)
      {
         case 0: f = ch(hash[1], hash[2], hash[3]); break;
         case 1:
         case 3: f = H(hash[1], hash[2], hash[3]); break;
         case 2: f = G(hash[1], hash[2], hash[3]); break;
      }
      const uint32_t tmp = Bits::rotateLeft(hash[0], 5, 32) + f + hash[4] + k[index] + int_block[j];
      hash[4] = hash[3];
      hash[3] = hash[2];
      hash[2] = Bits::rotateLeft(hash[1], 30, 32);
      hash[1] = hash[0];
      hash[0] = tmp;
   }
   
   applyDaviesMayerFunction(hash, state);
}