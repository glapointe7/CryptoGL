#include "SHA1.hpp"

#include "Bits.hpp"

constexpr uint32_t SHA1::IV[5];

const BytesVector SHA1::encode(const BytesVector &data)
{
   BytesVector bytes = appendPadding(data);
   appendLength<BigEndian64>(bytes, data.size() << 3);

   UInt32Vector states(IV, IV + 5);
   const uint64_t bits_len = bytes.size();
   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      UInt32Vector words = getInputBlocks(bytes, i);
      words.resize(80);

      // Extention of the 32-bits 16 blocks in 80 blocks of 32 bits.
      for (uint8_t j = 16; j < 80; ++j)
      {
         words[j] = Bits::rotateLeft(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1, 32);
      }

      UInt32Vector hash(states);
      uint32_t f, k;
      for (uint8_t j = 0; j < 80; ++j)
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

         const uint32_t tmp = Bits::rotateLeft(hash[0], 5, 32) + f + hash[4] + k + words[j];
         hash[4] = hash[3];
         hash[3] = hash[2];
         hash[2] = Bits::rotateLeft(hash[1], 30, 32);
         hash[1] = hash[0];
         hash[0] = tmp;
      }
      
      for (uint8_t j = 0; j < 5; ++j)
      {
         states[j] += hash[j];
      }
   }

   return getOutput(5, states);
}