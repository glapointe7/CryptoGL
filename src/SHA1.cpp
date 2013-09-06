#include "SHA1.hpp"

#include "Bits.hpp"

void SHA1::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   int_block.resize(rounds);

   // Extention of the 32-bits 16 blocks in 80 blocks of 32 bits.
   for (uint8_t j = 16; j < rounds; ++j)
   {
      int_block[j] = Bits::rotateLeft(int_block[j - 3] ^ int_block[j - 8] ^ int_block[j - 14] ^ int_block[j - 16], 1, 32);
   }

   UInt32Vector hash(state);
   uint32_t f, k;
   for (uint8_t j = 0; j < rounds; ++j)
   {
      if (j < 20)
      {
         f = ch(hash[1], hash[2], hash[3]);
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

      const uint32_t tmp = Bits::rotateLeft(hash[0], 5, 32) + f + hash[4] + k + int_block[j];
      hash[4] = hash[3];
      hash[3] = hash[2];
      hash[2] = Bits::rotateLeft(hash[1], 30, 32);
      hash[1] = hash[0];
      hash[0] = tmp;
   }
   
   for (uint8_t j = 0; j < 5; ++j)
   {
      state[j] += hash[j];
   }
}

const BytesVector SHA1::encode(const BytesVector &data)
{
   BytesVector bytes = appendPadding(data);
   bytes = appendLength<BigEndian64>(bytes, data.size() << 3);

   UInt32Vector states(IV);
   const uint64_t bits_len = bytes.size();
   for (uint64_t i = 0; i < bits_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, states);
   }

   return getOutput(states);
}